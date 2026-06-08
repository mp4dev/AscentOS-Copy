#include <stdlib.h>
#include <mpx/pcb.h>
#include <stddef.h>
#include <mpx/sys_call.h>
#include <mpx/iocb.h>
#include <mpx/device.h>
#include <mpx/dcb.h> // needed for enqueue
#include "mpx/sys_call.h"
#include "sys_req.h"
#include <memory.h>
struct pcb* pcbPtr = NULL; // points to the actively executing process

struct context* contextPtr = NULL; // represents the context from the first time sys_call is used

struct context *sys_call(struct context * current_context)
{
    
    // Checks if there is an active process and assigns it's context to the current
    if(pcbPtr != NULL)
    {
        pcbPtr->stackPtr = current_context;
    }
    IOComplete();
    int op = current_context->EAX;
    if (op == READ || op == WRITE) 
    {        

        device context_device = (device) current_context->EBX;
        dcb* context_dcb = find_dcb(context_device);
        char* buffer = (char*) current_context->ECX; // The buffer to read/write with.
        int size = current_context->EDX; // I assume this one isn't a pointer?

        iocb* new_iocb = create_iocb(op, context_dcb, buffer, size);
        IOScheduler(new_iocb);
        // While bytes are being transferred, block the pcb and start running a new one
        block_pcb(pcbPtr); // Blocks pcb, puts it in blocked queue. [Update to sys_call() 2/2 in pdf]
    }

    if (op == IDLE || op == READ || op == WRITE) // IDLE condition
    {
        if (contextPtr == NULL) // if sys_call function wasn't entered point to the currently executing process
        {
            contextPtr = current_context;
        }
        if (ready_head != NULL) // checks if ready queue is has something in it
        {
            pcb* temp = pop(&ready_head); // pop value into ptr

            if (pcbPtr != NULL && op == IDLE) {
                pcbPtr->stackPtr = current_context; // stores the address of the currently executing process
                push_by_priority(&ready_head, &ready_tail, pcbPtr); // enqueues 
            }

            pcbPtr = temp;
            pcbPtr->stackPtr->EAX = 0; // adjust register to exit
            return pcbPtr->stackPtr; // returns the modified value's address
        }
        if (ready_head == NULL && pcbPtr == NULL) // MUST ALSO CHECK THAT NO PROCESS IS CURRENTLY RUNNING
        {
            // Return original context if ready queue is null
            contextPtr->EAX = 0;
            context * helper = contextPtr;
            contextPtr = NULL;
            pcbPtr = NULL;
            return helper;
        }
        
        current_context->EAX = 0;
        return pcbPtr->stackPtr;
    }


    else if (op == EXIT) // EXIT condition
    {
        pcb_free(pcbPtr);
        pcbPtr = NULL;

        pcb* temp = pop(&ready_head);
        if (temp != NULL) // if something is in temp
        {
            pcbPtr = temp;
            pcbPtr->stackPtr->EAX = 0; // adjust pcbPtr to exit condition
            return pcbPtr->stackPtr;
        }
        else if (ready_head == NULL && pcbPtr == NULL) // Nothing in ready queue // MUST ALSO CHECK THAT NO PROCESS IS CURRENTLY RUNNING
        {
            contextPtr->EAX = 0;
            context * helper = contextPtr;
            contextPtr = NULL;
            pcbPtr = NULL;
            return helper;
        }
    }
    current_context->EAX = -1; // All other cases
    return current_context;
}

pcb* pop(struct pcb** queue_head)
{
    if (*queue_head == NULL) // can't remove anything if nothing is in the queue
    {
        return NULL;
    }
    pcb* temp = *queue_head; // stores the PCB at the top to return in temp
    *queue_head = (*queue_head)->next; // makes head pointer point to the next PCB in the queue 
    
    // if the head has something it's attach to make it point back to nothing
    if (*queue_head != NULL) 
    {
        (*queue_head)->prev = NULL;
    }

    // detaches PCB
    temp->next = NULL;
    temp->prev = NULL;

    return temp;
}

void push_by_priority(pcb** queue_head, pcb** queue_tail, struct pcb* process)
{
    if (process == NULL) 
        return;

    process->next = NULL;
    process->prev = NULL;

    // Empty queue
    if (*queue_head == NULL)
    {
        *queue_head = process;
        *queue_tail = process;
        return;
    }

    // Find the first node with lower priority (higher number)
    pcb* current = *queue_head;
    while (current != NULL && current->priority <= process->priority)
    {
        current = current->next;
    }

    // Insert before current
    if (current == NULL)
    {
        // Goes at the end
        process->prev = *queue_tail;
        (*queue_tail)->next = process;
        *queue_tail = process;
    }


    else if (current->prev == NULL)
    {
        // Goes at the front
        process->next = *queue_head;
        (*queue_head)->prev = process;
        *queue_head = process;
    }
    else
    {
        // Goes in the middle
        process->next = current;
        process->prev = current->prev;
        current->prev->next = process;
        current->prev = process;
    }
}

void IOScheduler(iocb* queued_iocb)
{
    dcb* dev = queued_iocb->d;
    if (dev->status == 0)
    {
        dev->curr_iocb = queued_iocb;
        if (dev->curr_iocb->operation == READ)
        {
            serial_read(dev->dev, queued_iocb->buffer, queued_iocb->buffer_size);
        }
        else if (dev->curr_iocb->operation == WRITE)
        {
            serial_write(dev->dev, queued_iocb->buffer, queued_iocb->buffer_size);
        }
    }
    else if (dev->status == IDLE)
    {
        enqueue_iocb(dev, queued_iocb);
    }
}

void IOComplete(void)
{
    for (int i = 0; i < DEVICECOUNT; i++)
    {
        dcb* curr_dcb = dcb_array[i];
        if (curr_dcb == NULL)
            continue;

        if (curr_dcb->event_flag != 1)
            continue;
        
        // First, Switch "Active process" from blocked to ready state. (Which one???)
        iocb* curr_iocb = curr_dcb->curr_iocb;
        if (curr_iocb == NULL) // This should probably never happen, but eh.
            continue;
        pcb* curr_process = curr_iocb->p;
        unblock_pcb(curr_process);
        // Second, set # of bytes transferred into that process' EAX as a return value
        curr_process->stackPtr->EAX = curr_iocb->buffer_size;
        // Third, Clear the active data structure, signaling that no request is currently active for this device. (The current IOCB?)
        sys_free_mem(curr_iocb);
        curr_dcb->curr_iocb = NULL; curr_iocb = NULL;
        // Finally, Search the IOCB waiting queue for another process waiting to use the newly available device and call IOScheduler
        if (curr_dcb->head == NULL)
            continue;

        curr_iocb = dequeue_iocb(curr_dcb);
        IOScheduler(curr_iocb);
    }
}
iocb* create_iocb(int op, dcb *curr_dcb, char *buffer, int size)
{
    iocb* new_iocb = (iocb*) sys_alloc_mem(sizeof(iocb));

    new_iocb->operation = op;
    new_iocb->d = curr_dcb;
    new_iocb->buffer = buffer;
    new_iocb->buffer_size = size;
    new_iocb->p = pcbPtr;
    return new_iocb;
}
