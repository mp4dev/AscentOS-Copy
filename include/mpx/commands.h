

#include <mpx/pcb.h>
#include <mpx/mcb.h>
#include <stdint.h>
/**
 @file commands.h
 @brief Command functions to be called by command hander
*/

/**
 * @brief Version command function
 * 
 * Prints current version of mpx project
 */
void version(void);

/**
 * @brief Help command function
 * 
 * Prints list of available commands.
 */
void help(void);

/**
 * @brief Additional help command function
 * 
 * Helper function that gives a more detailed description of a command.
 */
void help_extra(char *arg);

/**
 * @brief Shutdown command function
 * 
 * Exits out of command handler and shuts down operating system
 */
int shutdown(void);

/**
 * @brief Clear screen command function
 * 
 * Clears the terminal window of text
 */
void clear(void);

/**
 * @brief Get date function
 * 
 * Grabs the current date from the real time clock
 */
char *get_date(void);

/**
 * @brief Get time function
 * 
 * Grabs the current time from the real time clock
 */
char *get_time(void);

/**
 * @brief Set time function
 * 
 * Allows the user to set the time
 * @return A status code 0 if set successful, -1 if not
 */
int set_time(char time[16]);

/**
 * @brief Set date function
 * 
 * Allows the user to set the date
 * @return A status code 0 if set successful, -1 if not
 */
int set_date(char date[16]);

/**
 * @brief Create PCB function
 * 
 * Allows the user to create a PCB
 * @param name A unique and valid PCB name
 * @param class The class of PCB you want to create, either user or system
 * @param priority A prioirity level from 0-9, zero being the highest
 * 
 * @return A status code of 0 if the PCB is 
 *          created successfully, -1 if not
 */
int create_pcb(char *name, char *class, int priority);

/**
 * @brief Delete PCB function
 * 
 * Allows user to delete a pcb
 * @param name The name of an existing PCB
 * @return A status code of 0 if successful, -1 if not
 */
int delete_pcb(char *name);



/**
 * @brief Suspend pcb function
 * 
 * Allows a user to suspend a pcb
 * @param name The name of an existing PCB
 * @return A status code of 0 if successful, -1 if not
 */
int suspend_pcb(char *name);

/**
 * @brief Resume pcb function
 * 
 * Allows a user to resume a pcb, moving it from the suspended to not suspended state
 * @param name The name of an existing PCB
 * @return A status code of 0 if successful, -1 if not
 */
int resume_pcb(char *name);

/**
 * @brief Set pcb priority function
 * 
 * Allows a user to set the priority value of a given pcb
 * @param name The name of an existing PCB
 * @param priority A prioirity level from 0-9, zero being the highest
 * @return A status code of 0 if successful, -1 if not
 */
int set_pcb_priority(char *name, int priority);

/**
 * @brief A helper function to validate a PCB name
 * 
 * Checks to see if a provided name is 8 or less characters. If not, it prints an error message.
 * @param name The name of a PCB
 * @return A status code of 0 if the name is valid, -1 if not
 */
int pcb_validate_name(char *name);

void print_pcb(pcb* currpcb);

void show_pcb(char* name);

void show_ready(void);

void show_blocked(void);

void show_all(void);

void pcb_print_queues(int print_R, int print_RS, int print_B, int print_BS);

void yield(void);

/**
 * @brief Create PCB function
 * 
 * Allows the user to create a PCB
 * @param name A unique and valid PCB name
 * @param class The class of PCB you want to create, either user or system
 * @param priority A prioirity level from 0-9, zero being the highest
 * 
 * @return returns the pointer to the pcb struct that was created
 */
pcb* create_pcb_no_insert(char *name, char *class, int priority);

/**
 * @brief Initializes the CPU context for a process
 * Creates the context structure and then inserts it onto the pcb stack
 * @param process Pointer to the PCB whose context will be initialized
 * @param func Pointer to the process function that is loaded to the EIP
 */
void insert_context(pcb* process, void(*func)(void));

/**
 * @brief Used to load the processes
 * Loads the processes from the process.h file and inserts them into the appropriate queue
 * @param disp_state Determines whether to load the processes as Suspended or Non-Suspended
 */
void load_r3(int disp_state);

/**
 * @brief Used to load the process by its name
 * Loads the process that is named from the process.h file and inserts it into the appropriate queue
 * @param name Determines which process to load from process.h
 */
void load_process(char* name);

/**
 * @brief resumes all ready suspended and blocked suspended processes
 * Resumes processes that were in the ready suspended and blocked suspended queues by moving them to the ready and blocked queues.
 */
void resume_all(void);

int set_alarm(char time[16], char* message);
/**
 * @brief allocates memory
 * 
 * Allocates memory based on the size passed to it 
 * @param size Size of the memory wanting to be allocated
 * 
 * @return returns 0 if successful or -1 if unable to allocate the memory
 */
int alloc_mem(int size);

/**
 * @brief frees memory
 * 
 * Frees the specified memory block 
 * @param block_addr Address of the block address that is to be freed
 * 
 * @return returns 0 if successful or -1 if unable to free the memory
 */
int free_mem(char *block_addr);

void print_mcbs(int flag);

void print_mcb(mcb* curr_mcb);
