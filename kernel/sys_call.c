#include <stdlib.h>
#include <mpx/pcb.h>
#include <stddef.h>
#include <mpx/sys_call.h>

struct pcb* pcbPtr = NULL; // points to the actively executing process

struct context* contextPtr = NULL; // represents the context from the first time sys_call is used

struct context *sys_call(struct context * current_context)
{
    // Checks if there is an active process and assigns it's context to the current
    if(pcbPtr != NULL)
    {
        pcbPtr->stackPtr = current_context;
    }

    if (current_context->EAX == 1) // IDLE condition
    {
        if (contextPtr == NULL) // if sys_call function wasn't entered point to the currently executing process
        {
            contextPtr = current_context;
        }
        if (ready_head != NULL) // checks if ready queue is has something in it
        {
            pcb* temp = pop(&ready_head); // pop value into ptr

            if (pcbPtr != NULL) {
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


    else if (current_context->EAX == 0) // EXIT condition
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
