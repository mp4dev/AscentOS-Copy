#include <stddef.h>
#include <mpx/pcb.h>
#include "memory.h"
#include "mpx/serial.h"
#include <string.h>

pcb *ready_head = NULL; 
pcb *ready_tail = NULL;
pcb *ready_sus_head = NULL; 
pcb *ready_sus_tail = NULL;
pcb *blocked_head = NULL; 
pcb *blocked_tail = NULL;
pcb *blocked_sus_head = NULL; 
pcb *blocked_sus_tail = NULL;

pcb* pcb_allocate(void)
{
  struct pcb* new_pcb = (struct pcb*)(sys_alloc_mem(sizeof(struct pcb)));

  // Allocation Failed
  if(!new_pcb)
  {
    return NULL; // Can't return NULL without <stddef.h>, <stdio.h>, or <stdlib.h>
  }
  
  // Fills stack with null terminators
  for(size_t i = 0; i < sizeof(new_pcb->stack) / sizeof(int); i++)
  {
    (*new_pcb).stack[i] = '\0';
  }

  // returns the newly created pcb struct
  return new_pcb;
}


struct pcb* pcb_setup(char * process_name, int class, int priority)
{
    // Initializes default values of the new pcb
    pcb* new_pcb = pcb_allocate();
    new_pcb->priority = priority;
    new_pcb->ex_state = READY;
    new_pcb->disp_state = NOT_SUSPENDED;
    new_pcb->class = class;
    
    // Stores the string for the process name inside the name portion of the pcb struct
    int i;
    for (i = 0; i < CHAR_LIMIT-1; i++)
    {
        if (process_name[i] == '\0') // Breaks out of loop if name is shorter than 8 characters
        {
            break;
        }
        new_pcb->name[i] = process_name[i]; // Assigns the process name to the struct name character by character
    }
    new_pcb->name[i] = '\0'; // Ends name array with null terminator to ensure no empty memory spots
    return new_pcb;
}

int pcb_free(struct pcb* pcb)
{
  // Checks again if the pcb is invalid pcb == NULL
  if (!pcb)
  {
    return -1; // Returns -1 if it isn't valid
  }

  // Frees memory and returns 0 if pcb is valid
  sys_free_mem(pcb);
  return 0;
}

void fifo_node(pcb **head, pcb **tail, pcb *node){
    node->next = NULL;
    node->prev = NULL;
    //if queue is empty
    if(*head == NULL){
        *head = node;
        *tail = node;
        return;
    }
    //insert at the end of queue
    node->prev = *tail;
    (*tail)->next = node;
    *tail = node;
}

void priority_node(pcb **head, pcb **tail, pcb *node){
    pcb *current;
    node->next = NULL;
    node->prev = NULL;
  //if queue is empty
    if(*head == NULL){
        *head = node;
        *tail = node;
        return;
    }
    current = *head;
    //keeps iterating until current becomes null or node is greater than the current 
    while(current != NULL && current->priority <= node->priority){
        current = current->next;
    }
    //Places node at the head
    if(current == *head){
        node->next = *head;
        (*head)->prev = node;
        *head = node;
        return;
    }
    //Places node at the tail
    if(current == NULL){
        node->prev = *tail;
        (*tail)->next = node;
        *tail = node;
        return;
    }
    //Places node in queue
    node->next = current;
    node->prev = current->prev;
    current->prev->next = node;
    current->prev = node;
    return;
}



struct pcb* pcb_find(const char *name){
    struct pcb *current;
    //Ready Queue
    current = ready_head;
    while(current!=NULL){
        if(strcmp(current->name,name)==0){
            return current;
        }
        current = current->next;
    }
    //Ready Suspended Queue
    current = ready_sus_head;
    while(current!=NULL){
        if(strcmp(current->name,name)==0){
            return current;
        }
        current = current->next;
    }
    //Blocked Queue
    current = blocked_head;
    while(current!=NULL){
        if(strcmp(current->name,name)==0){
            return current;
        }
        current = current->next;
    }
    //Blocked Suspened Queue
    current = blocked_sus_head;
    while(current!=NULL){
        if(strcmp(current->name,name)==0){
            return current;
        }
        current = current->next;
    }
    return NULL;
 }

 void pcb_insert(struct pcb* name){
    struct pcb **head;
    struct pcb **tail;
    //node allocation
    name->prev = NULL;
    name->next = NULL;
    //Ready Queue
    if(name->ex_state == READY || name->ex_state == RUNNING){
        if(name->disp_state == NOT_SUSPENDED){
            head = &ready_head;
            tail = &ready_tail;
        }
        //Ready Suspended Queue
        else{
            head = &ready_sus_head;
            tail = &ready_sus_tail;
        }
        priority_node(head, tail, name);
        return;
    }
    //Blocked Queue
    if(name->ex_state == BLOCKED){
        if(name->disp_state == NOT_SUSPENDED){
            head = &blocked_head;
            tail = &blocked_tail;
        }
        //Blocked Suspended Queue
        else{
            head = &blocked_sus_head;
            tail = &blocked_sus_tail;
        }
        fifo_node(head, tail, name);
        return;
    }
    return;//if for some reason it falls through, it will still return
 }

 int pcb_remove(struct pcb* name){
    struct pcb **head;
    struct pcb **tail;
    //Ready Queue
    if(name->ex_state == READY || name->ex_state == RUNNING){
        if(name->disp_state == NOT_SUSPENDED){
            head = &ready_head;
            tail = &ready_tail;
        }
        //Ready Suspended Queue
        else{
            head = &ready_sus_head;
            tail = &ready_sus_tail;
        }
    }
    //Blocked Queue
    else if(name->ex_state == BLOCKED){
        if(name->disp_state == NOT_SUSPENDED){
            head = &blocked_head;
            tail = &blocked_tail;
        }
        //Blocked Suspended Queue
        else{
            head = &blocked_sus_head;
            tail = &blocked_sus_tail;
        }
    }
    else{
        return -2; //Couldn't find state
    }

    //actually removing
    if(name->prev != NULL){
        name->prev->next = name->next;
    }
    else{
        *head = name->next;
    }
    if(name->next != NULL){
        name->next->prev = name->prev;
    }
    else{
        *tail = name->prev;
    }
    return 0;//not found
 }

char* get_exec_state(int state)
{
    switch(state)
    {
        case READY:   return "READY";
        case RUNNING: return "RUNNING";
        case BLOCKED: return "BLOCKED";
        default:      return "NOT A VALID EXEC STATE";
    }
}

char* get_disp_state(int state)
{
    switch(state)
    {
        case SUSPENDED:     return "SUSPENDED";
        case NOT_SUSPENDED: return "NOT SUSPENDED";
        default:            return "NOT A VALID DISP STATE";
    }
}

char* get_class(int class)
{
    switch(class)
    {
        case USER:   return "USER";
        case SYSTEM: return "SYSTEM";
        default:     return "UNKNOWN";
    }
}
