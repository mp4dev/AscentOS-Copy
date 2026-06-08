#ifndef PCB_H
#define PCB_H

typedef enum {
  READY,
  RUNNING,
  BLOCKED,
  SUSPENDED,
  NOT_SUSPENDED
} state;

// Defines user and system level processes
typedef enum {
  USER,
  SYSTEM
} class;

// The max length of name
#define CHAR_LIMIT 10

#define STACK_SIZE 1024
// The process control block where all new instances of processes are made
typedef struct pcb {
    char name[CHAR_LIMIT]; // The max size the name of the struct is 8 chars
    int class; // 0=user, 1=system
    int priority; // 0-9, high-low
    int ex_state; // 0=ready, 1=running, 2=blocked
    int disp_state; // 3=suspened, 4=not suspended
    unsigned char stack[STACK_SIZE]; // OUR STACK SIZE IS 1024 BYTES OR 8192 BITS!!!
    struct context* stackPtr;
    struct pcb *next;
    struct pcb *prev;
} pcb;

extern pcb *ready_head; 
extern pcb *ready_tail;
extern pcb *ready_sus_head; 
extern pcb *ready_sus_tail;
extern pcb *blocked_head; 
extern pcb *blocked_tail;
extern pcb *blocked_sus_head; 
extern pcb *blocked_sus_tail;

// Holds register information for context switching
typedef struct context {
  int DS;
  int ES;
  int FS;
  int GS;
  int SS;

  int EDI;
  int ESI;
  int EBP;
  int EBX;
  int EDX;
  int ECX;
  int EAX;

  int EIP;
  int CS;
  int EFLAGS;

}context;




/**
 * @brief Used for resolving data into a newly created instance of a struct
 * @param process_name The string you want name the instance of a struct
 * @param class Assigns an instance of a process to a user or system level
 * @param priority Assigns a struct's priority to this parameter
 * @return An integer containing -1 for failure and 0 for success
 */
struct pcb* pcb_setup(char * process_name, int class, int priority);

/**
 * @brief Used for creating the instance of a pcb struct and instantiating data members
 * @return A instance of a newly created pcb struct
 */
struct pcb* pcb_allocate(void);

/**
 * @brief Used for freeing memory associated with pcb structs
 * @param pcb A pointer to an instance of a pcb
 * @return An integer containing -1 for failure and 0 for success
 */
int pcb_free(struct pcb*);

/**
 * @brief Inserts a node at the end of a queue using First-In-First-Out (FIFO) ordering.
 *
 * This function appends the provided node to the tail of the specified
 * doubly linked list. If the queue is empty, the node becomes both the
 * head and tail of the list.
 *
 * @param head  Pointer to the head of the queue.
 * @param tail  Pointer to the tail of the queue.
 * @param node  Pointer to the Node to insert.
 */
void fifo_node(pcb **head, pcb **tail, pcb *node);

/**
 * @brief Inserts a node into a queue based on priority ordering.
 *
 * This function inserts the provided node into the specified doubly
 * linked list according to its priority value. Lower numerical priority
 * values represent higher priority (0 = highest priority). If the queue is
 * empty, the node becomes both the head and tail of the list.
 *
 * @param head  Pointer to the head of the queue.
 * @param tail  Pointer to the tail of the queue.
 * @param node  Pointer to the Node to insert.
 */
void priority_node(pcb **head, pcb **tail, pcb *node);

/**
 * @brief Searches all process queues for a PCB with the given name.
 *
 * Iterates through the ready, blocked, suspended, and blocked-suspended
 * queues and compares each PCB name to the provided string.
 *
 * @param name The name of the PCB to search for.
 *
 * @return Pointer to the matching PCB if found or NULL if no PCB with the given name exists.
 */
struct pcb* pcb_find(const char *name);

/**
 * @brief Inserts a PCB into the appropriate queue.
 *
 * Allocates a new Node and inserts the PCB into either the FIFO
 * or priority queue depending on its state and priority.
 *
 * @param name Pointer to the PCB to insert.
 */
void pcb_insert(struct pcb* name);

/**
 * @brief Removes a PCB from its current queue.
 *
 * Locates the PCB in its queue and removes the associated Node,
 * updating head and tail pointers as necessary.
 *
 * @param name Pointer to the PCB to remove.
 *
 * @return 0 on success or a negative number if the PCB is not found or error.
 */
int pcb_remove(struct pcb* name);

/**
 * @brief Block pcb function
 * 
 * Allows a user to block a pcb
 * @param name The name of an existing PCB
 * @return A status code of 0 if successful, -1 if not
 */
int block_pcb(pcb* pcb_to_block);

/**
 * @brief Unblock pcb function
 * 
 * Allows user to unblock a pcb
 * @param name The name of an existing PCB
 * @return A status code of 0 if successful, -1 if not
 */
int unblock_pcb(pcb* pcb_to_unblock);


/**
 * @brief Used to get the string version of the pcb execution enum
 * @param state An integer representing the pcb execution state
 * @return A char* that represents the pcb execution state enum passed in.
 */
char* get_exec_state(int state);

/**
 * @brief Used to get the string version of the pcb dispatch enum
 * @param state An integer representing the pcb distpatch enum
 * @return A char* that represents the pcb dispatch state enum passed in.
 */
char* get_disp_state(int state);

/**
 * @brief Used to get the string version of the state enum
 * @param state An integer representing the pcb state
 * @return A char* that represents the execution state enum passed in.
 */
char* get_class(int class);


#endif
