#include <mpx/dcb.h>
#include <mpx/io.h>
/**
 * @brief Used for processing part of the ISR. Using EAX registers for each process, it is involved in modifying 
 * each ready, running, and nonsuspended queues based on IDLE and EXIT conditions on aformentioned register. Applies context
 * switching
 * @param current_process the PCB you want to have an ISR processed for
 * @return a pointer to the context to be loaded with EAX vals
 */
struct context *sys_call(struct context*);

/**
 * @brief removes a single PCB from the top of a queue
 * @param queue_head a pointer to the top of the doubly linked list queue
 * @return the PCB that was removed from the top of the queue
 */
pcb* pop(struct pcb** queue_head);

/**
 * @brief enqueues a single process at correct location in the queue based on priority
 * @param queue_head a pointer to the top of the doubly linked list queue
 * @param queue_tail a pointer to the end of the doubly linked list queue
 * @param queue_process the PCB that you want to push at the end of the queue
 */
void push_by_priority(pcb** queue_head, pcb** queue_tail, struct pcb* process);

/// @brief 
/// @param op 
/// @param dcb 
/// @param buffer 
/// @param size 
void IOScheduler(iocb* queued_iocb);

void IOComplete(void);

iocb *create_iocb(int op, dcb *curr_dcb, char *buffer, int size);
