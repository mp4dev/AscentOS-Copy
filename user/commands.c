
#include <mpx/commands.h>
#include <../include/stdlib.h> // some compilers assumed functions declared in standard stdlib.h if not delcared explicitly
#include <sys_req.h>
#include <string.h>
#include <mpx/functions.h>
#include <mpx/color.h>
#include <mpx/print.h>
#include <mpx/pcb.h>
#include <processes.h>
#include <mpx/alarm.h>

/**
 * @def VERSION
 * @brief Current Module/Release of mpx project
 */
#define VERSION "R6"

int century=20;//used to grab the first 2 numbers of the year, because RTC only stores the last 2

// R1 functions

void version(void)
{
    print_col("Version " VERSION ", Compilation Date: "__DATE__"\n", YELLOW);
}

void help(void)
{
    print_col("Command Information [Arguments are written in brackets, do not include brackets]:\n---------- System Utilites ----------\n", YELLOW);
    print(
        "  help [cmd] - Shows available commands, if additional command is provided, shows a detailed description\n"
        "  version - Shows version and time of compilation\n"
        "  shutdown - Shuts down OS after confirmation\n"
        "  clear - Clears the terminal window of text\n"
        "  get_date - Shows OS date\n"
        "  set_date <date> - Sets the OS date to the date provided using mm/dd/yyyy format\n"
        "  get_time - Shows OS time\n"
        "  set_time <time> - Sets the OS time to the time provided using hh:mm:ss format\n"
        "  alarm <time> - Sets an alarm for the time provided using hh:mm:ss format and a message that will be displayed when the alarm triggers.\n"
    );
    print_col("---------- Process Management ----------\n*For PCB commands, you may omit \'_pcb\'\n", YELLOW);
    print(
        "  create_pcb \x1b[31m~~DEPRECATED~~\x1b[0m\n"
        "  delete_pcb <name> - Deletes the PCB with the given name\n"
        "  block_pcb \x1b[31m~~DEPRECATED~~\x1b[0m\n"
        "  unblock_pcb \x1b[31m~~DEPRECATED~~\x1b[0m\n"
        "  suspend_pcb <name> - Suspends the PCB with the given name\n"
        "  resume_pcb <name> - Unsuspends the PCB with the given name\n"
        "  set_priority <name> <priority> - Modifies the priority of a PCB and moves it to the appropriate location\n"
        "  show_pcb <name> - Displays the pcb's name, priority, class, execution status,suspend status\n"
        "  show_ready - Displays the information of every pcb that is in the ready state\n"
        "  show_blocked - Displays the information of every pcb that is in the blocked state\n"
        "  show_all - Displays the information of every pcb \n"
        "  yield \x1b[31m~~DEPRECATED~~\x1b[0m\n"
        "  load_r3 <state> - Loads the R3 test processes as eiter \"ready\" or \"suspended\"\n"
        "  load_process <name> - Loads the process with the given name as ready non-suspended\n"
        "  resume_all - Resumes all ready suspended processes\n"
    );
    print_col("---------- Memory Management ----------\n", YELLOW);
    print(
        "  allocate_mem \x1b[31m~~DEPRECATED~~\x1b[0m\n"
        "  free_mem \x1b[31m~~DEPRECATED~~\x1b[0m\n"
        "  show_allocated - Displays the size and start address of all allocated blocks\n"
        "  show_free - Displays the size and start address of all free blocks\n"
    );
}

void help_extra(char *arg) {
    if (strcmp(arg ,"help") == 0) {
        print(
            "  command: help\n"
            "  arguments: [cmd] (optional) - A name of a command\n"
            "  Shows available commands. If optional command argument is provided, gives a detailed description of\n"
            "  the provided command.\n"
        );
    }
    else if (strcmp(arg ,"version") == 0 || strcmp(arg ,"v") == 0) {
        print(
            "  command: version OR v\n"
            "  arguments: none\n"
            "  Shows version and time of compilation.\n"
        );
    }
    else if (strcmp(arg ,"shutdown") == 0) {
        print(
            "  command: shutdown\n"
            "  arguments: none\n"
            "  Shuts down operating system after confirmation by user.\n"
        );
    }
    else if (strcmp(arg ,"clear") == 0) {
        print(
            "  command: clear\n"
            "  arguments: none\n"
            "  Clears the terminal window of text by moving cursor and screen position,\n"
            "  not by deleting characters.\n"
        );
    }
    else if (strcmp(arg ,"get_date") == 0) {
        print(
            "  command: get_time\n"
            "  arguments: none\n"
            "  Shows OS date\n"
        );
    }
    else if (strcmp(arg ,"set_date") == 0) {
        print(
            "  command: set_date\n"
            "  arguments: <date> (required) - a valid date, formatted as mm/dd/yyyy\n"
            "  Sets the OS date to the date provided using mm/dd/yyyy, where mm\n"
            "  indicates the month, dd indicates the day, and yyyy indicates the year.\n"
        );
    }
    else if (strcmp(arg ,"get_time") == 0) {
        print(
            "  command: get_time\n"
            "  arguments: none\n"
            "  Shows OS time\n"
        );
    }
    else if (strcmp(arg ,"set_time") == 0) {
        print(
            "  command: set_time\n"
            "  arguments: <time> (required) - a valid time on a 24 hour clock, formatted as hh:mm:ss\n"
            "  Sets the OS time to the time provided using hh:mm:ss format, where hh indicates \n"
            "  the hour in a 24 hour clock, mm indicates the minute, and ss indicates the second.\n"
        );
    }
    // else if (strcmp(arg, "create_pcb") == 0) {
    //     print(
    //         "  command: create_pcb\n"
    //         "  arguments: <name> (required) - a valid name\n"
    //         "             <class> (required) - a pcb class, either user or system\n"
    //         "             <priority> (required) - a priority integer between 0-9\n"
    //         "  Creates a new pcb with the given attributes, and inserts it into the ready queue.\n"
    //     );
    // }
    else if (strcmp(arg, "delete_pcb") == 0) {
        print(
            "  command: delete_pcb\n"
            "  arguments: <name> (required) - a name of an existing PCB\n"
            "  Deletes the PCB with the provided name, given that it is not a system process.\n"
        );
    }
    else if (strcmp(arg, "block_pcb") == 0) {
        print(
            "  command: block_pcb\n"
            "  arguments: <name> (required) - a name of an existing PCB\n"
            "  Puts the PCB with the provided name in the blocked state, given that it is not a system process.\n"
        );
    }
    else if (strcmp(arg, "unblock_pcb") == 0) {
        print(
            "  command: unblock_pcb\n"
            "  arguments: <name> (required) - a name of an existing PCB\n"
            "  Puts the PCB with the provided name in the unblocked state, given that it was blocked.\n"
        );
    }
    else if (strcmp(arg, "suspend_pcb") == 0) {
        print(
            "  command: suspend_pcb\n"
            "  arguments: <name> (required) - a name of an existing PCB\n"
            "  Puts the PCB with the provided name in the suspended state, given that it is not a system process.\n"
        );
    }
    else if (strcmp(arg, "resume_pcb") == 0) {
        print(
            "  command: resume_pcb\n"
            "  arguments: <name> (required) - a name of an existing PCB\n"
            "  Puts the PCB with the provided name in the not-suspended state, given that it was suspended.\n"
        );
    }
    else if (strcmp(arg, "set_priority") == 0) {
        print(
            "  command: set_priority\n"
            "  arguments: <name> (required) - a name of an existing PCB\n"
            "             <priority> (required) - a priority integer between 0-9\n"
            "  Changes the priority of a process, and moves it to the appropriate location in the appropriate queue.\n"
        );
    }
    else if (strcmp(arg, "show_pcb") == 0) {
        print(
            "  command: show_pcb\n"
            "  arguments: <name> (required) - a name of an existing PCB\n"
            "  If the pcb is found, it displays the pcb's name, priority, class, execution status, and suspend status.\n"
        );
    }
    else if (strcmp(arg, "show_ready") == 0) {
        print(
            "  command: show_ready\n"
            "  arguments: none\n"
            "  Displays every ready and ready suspended pcb with each pcb's name, priority, class, execution status, and suspend status.\n"
        );
    }
    else if (strcmp(arg, "show_blocked") == 0) {
        print(
            "  command: show_blocked\n"
            "  arguments: none\n"
            "  Displays every blocked and blocked suspended pcb with each pcb's name, priority, class, execution status, and suspend status.\n"
        );
    }
    else if (strcmp(arg, "show_all") == 0) {
        print(
            "  command: show_all\n"
            "  arguments: none\n"
            "  Displays every ready, blocked, ready suspended, and blocked suspended pcb with each pcb's name, priority, class, execution status, and suspend status. \n"
        );
    }
    else if (strcmp(arg, "alarm") == 0) {
        print(
            "  command: alarm\n"
            "  arguments: <time> (required) - enter the time wanted for the alarm in the format HH:MM:SS\n"
            "             <message> (prompted) - after being prompted, enter a message that you want to be displayed when the alarm triggers\n"
            "  Creates an alarm with a message that will trigger when the entered time has occurred. \n"
        );
    }

    else if (strcmp(arg, "load_r3") == 0) {
        print(
            "  command: load_r3\n"
            "  arguments: <state> (required) - a display state to load processes in: ready/suspended\n"
            "  Creates processes 1-5 and places them in the correct queue based on state arg passed by user.\n"
        );
    }
    else if (strcmp(arg, "load_process") == 0) {
        print(
            "  command: load_process\n"
            "  arguments: <name> (required) - name of process to be loaded\n"
            "  Creates and loads the individual process and places in the ready queue.\n"
        );
    }
    else if (strcmp(arg, "resume_all") == 0) {
        print(
            "  command: resume_all\n"
            "  arguments: none\n"
            "  Moves all processes into the ready queue.\n"
        );
    }
//     else if (strcmp(arg, "yield") == 0) {
//         print(
//             "  command: yield\n"
//             "  arguments: none\n"
//             "  Makes commhand yield the CPU to the currently ready processes.\n"
//         );
//     }
    // else if (strcmp(arg, "allocate_mem") == 0) {
    //     print(
    //         "  command: allocate_mem\n"
    //         "  arguments: <size> (required) - the size of a memory block to allocate in decimal\n"
    //         "  Allocates an MCB of the provided size to the heap. Displays the starting address if successful.\n"
    //     );
    // }
    // else if (strcmp(arg, "free_mem") == 0) {
    //     print(
    //         "  command: free_mem\n"
    //         "  arguments: <address> (required) - the hexadecimal start address of a memory block to delete\n"
    //         "  Frees the MCB with the provided start address, if it exists.\n"
    //     );
    // }
    else if (strcmp(arg, "show_allocated") == 0) {
        print(
            "  command: show_allocated\n"
            "  arguments: none\n"
            "  Displays all allocated MCBs, with both their starting address and size.\n"
        );
    }
    else if (strcmp(arg, "show_free") == 0) {
        print(
            "  command: show_free\n"
            "  arguments: none\n"
            "  Displays all free MCBs, with both their starting address and size.\n"
        );
    }
    else { // Default
        print_err("Provided command for help [cmd] not recognized. Type \"help\" to see available commands.\n");
    }
}

int shutdown(void)
{
    print_col("Are you sure you want to shutdown? Type y to confirm:\n", YELLOW);

    // Read next byte for shutdown confirmation
    char buf[1] = { 0 };
    int nread = sys_req(READ, COM1, buf, sizeof(buf));

    if (nread == 1 && buf[0] != '\n' && buf[0] == 'y') {
        print("\n");
        return 0; // shutdown confirmed
    }
    print("\n");
    return -1; // shutdown not confirmed
}

void clear(void) {
    print("\033[H"); // Move cursor to top left
    print("\033[2J"); // Clear entire screen
    // print("\033[3J"); // Erase scrollback buffer
}

char *get_date(void)
{
    int day = kernel_get_day();
    int month = kernel_get_month();
    int year = kernel_get_year();
    year = (century * 100) + year;
    static char date[12];
    date[0]=month/10 + '0';
    date[1]=month%10 + '0';
    date[2]='/';
    date[3]=day/10 + '0';
    date[4]=day%10 + '0';
    date[5]='/';
    date[6]=year/1000 + '0';
    date[7]=(year/100)%10 + '0';
    date[8]=(year/10)%10 + '0';
    date[9]=year%10 + '0';
    date[10]='\n';
    date[11]='\0';
    return date;
}

char *get_time(void)
{
    int second = kernel_get_second();
    int minute = kernel_get_minute();
    int hour = kernel_get_hour();
    static char time[10];  
    time[0]=hour/10 + '0';
    time[1]=hour%10 + '0';
    time[2]=':';
    time[3]=minute/10 + '0';
    time[4]=minute%10 + '0';
    time[5]=':';
    time[6]=second/10 + '0';
    time[7]=second%10 + '0';
    time[8]='\n';
    time[9]='\0';
    return time;
}

int set_time(char time[16])
{   
    //checking to make sure the correct delimiter is used
    int correct_delim=0;
    if(time[2]==':'){
            correct_delim++;
        }
    if(time[5]==':'){
            correct_delim++;
        }
    if(correct_delim!=2){
        print_err("Invalid entry for your delimiter. Please use ':' to break up the time and make sure it's in the correct spot. It should be HH:MM:SS\n");
        return -1; 
    }
    int second, minute, hour;
    char *tok = strtok(time, ":");
    hour = atoi(tok);
    tok = strtok(NULL, ":");
    minute = atoi(tok);
    tok = strtok(NULL, "/");
    second = atoi(tok);

    // Checking to see if the second, minute, and hour is valid
    if (second < 0 | second > 60)
    {
        print_err("Invalid entry for second. Please enter between the range of 0-60\n");
        return -1;
    }
    if (minute < 0 || minute > 60)
    {
        print_err("Invalid entry for minute. Please enter between the range of 0-60\n");
        return -1;
    }
    if (hour < 0 || hour > 23)
    {
        print_err("Invalid entry for hour. Please enter between the range of 1-24\n");
        return -1;
    }

    kernel_set_time_second(second);
    kernel_set_time_minute(minute);
    kernel_set_time_hour(hour);
    return 0; // set successful
}

int set_date(char date[16])
{
    //checking to make sure the correct delimiter is used
    int correct_delim=0;
    if(date[2]=='/'){
            correct_delim++;
        }
    if(date[5]=='/'){
            correct_delim++;
        }
    if(correct_delim!=2){
        print_err("Invalid entry for your delimiter. Please use '/' to break up the date. It should be MM/DD/YYYY.\n");
        return -1; 
    }
    int month, day, year;
    int leap_year = 0;
    char *tok = strtok(date, "/");
    month = atoi(tok);
    tok = strtok(NULL, "/");
    day = atoi(tok);
    tok = strtok(NULL, "/");
    year = atoi(tok);

    // Checking to see if the entered month is in the correct range from 1-12
    if (month < 1 || month > 12)
    {
        print_err("Invalid entry for month. Please enter between the range of 1-12\n");
        return -1;
    }

    // Finding if the year is a leap year to determine if the day entered is valid
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
    {
        leap_year = 1;
    }

    if (month == 2 && leap_year == 1)
    {
        if (day < 1 || day > 29)
        {
            print_err("Invalid entry for day. Please enter between the range of 1-29\n");
            return -1;
        }
    }

    // Checking to make sure day is valid if non-leap year
    if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        if (day < 1 || day > 30)
        {
            print_err("Invalid entry for day. Please enter between the range of 1-30\n");
            return -1;
        }
    }
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
    {
        if (day < 1 || day > 31)
        {
            print_err("Invalid entry for day. Please enter between the range of 1-31\n");
            return -1;
        }
    }

    if (month == 2 && leap_year == 0)
    {
        if (day < 1 || day > 28)
        {
            print_err("Invalid entry for day. Please enter between the range of 1-28\n");
            return -1;
        }
    }

    //Fixing the year so that it inputs correctly to the RTC
    century = year / 100;
    year = year % 100;
    kernel_set_date_month(month);
    kernel_set_date_day(day);
    kernel_set_date_year(year);
    return 0; // set successful
}

// R2 functions - Disabled create_pcb for R3

// int create_pcb(char *name, char *class, int priority) {
//     // Ensure PCB does not already exist
//     if (pcb_find(name) != NULL) {
//         print_err("Error: PCB with given name already exists.\n");
//         return -1;
//     }

//     // Ensure name is valid
//     if (pcb_validate_name(name) != 0) {
//         return -1;
//     }
    
//     // Process class
//     int class_enum;
//     if (strcmp(class, "user") == 0 ) {
//         class_enum = USER;
//     }
//     else if (strcmp(class, "system") == 0) {
//          class_enum = SYSTEM;
//     }
//     else {
//         print_err("Error: PCB class must be either user or system.\n");
//         return -1;
//     }

//     // Priority must be valid (0-9)
//     if (priority < 0 || priority > 9) {
//         print_err("Error: PCB priority must be between 0-9.\n");
//         return -1;
//     }

//     pcb* new_pcb = pcb_setup(name, class_enum, priority);

//     // use pcb insert to insert into correct queue
//     pcb_insert(new_pcb);

//     return 0;
// }

int delete_pcb(char *name) {
    // Check for valid name
    if (pcb_validate_name(name) != 0) {
        return -1;
    }

    // Ensure PCB exists
    pcb *pcb_to_delete = pcb_find(name);
    if (pcb_to_delete == NULL) {
        print_err("Error: PCB with given name does not exist.\n");
        return -1;
    }

    // Must NOT be a system process
    if (pcb_to_delete->class == SYSTEM) {
        print_err("Error: A system process cannot be deleted.\n");
        return -1;
    }

    // remove it from queue
    if (pcb_remove(pcb_to_delete) != 0) {
        print_err("Error encountered when attempting to remove PCB from queue\n");
        return -1;
    }

    // free memory
    if (pcb_free(pcb_to_delete) != 0) {
        print_err("Error encountered when attempting to free PCB memory\n");
        return -1;
    }

    return 0;
}



int suspend_pcb(char *name) {
    // Check for valid name
    if (pcb_validate_name(name) != 0) {
        return -1;
    }

    // Ensure PCB exists
    pcb *pcb_to_suspend = pcb_find(name);
    if (pcb_to_suspend == NULL) {
        print_err("Error: PCB with given name does not exist.\n");
        return -1;
    }

    // Must NOT be a system process
    if (pcb_to_suspend->class == SYSTEM) {
        print_err("Error: A system process cannot be suspended.\n");
        return -1;
    }

    // Ensure not already suspended
    if (pcb_to_suspend->disp_state == SUSPENDED) {
        print_err("Error: PCB already suspended.\n");
        return -1;
    }

    // Remove from queue
    if (pcb_remove(pcb_to_suspend) != 0) {
        print_err("Error encountered when attempting to remove PCB from queue.\n");
        return -1;
    }

    // Change dispatch state to suspended
    pcb_to_suspend->disp_state = SUSPENDED;

    // Reinsert
    pcb_insert(pcb_to_suspend);

    return 0;
}

int resume_pcb(char *name) {
    // Ensure name is valid
    if (pcb_validate_name(name) != 0) {
        return -1;
    }

    pcb *pcb_to_resume = pcb_find(name);
    if (pcb_to_resume == NULL) {
        print_err("Error: PCB with given name does not exist.\n");
        return -1;
    }

    // Ensure not already not suspended
    if (pcb_to_resume->disp_state == NOT_SUSPENDED) {
        print_err("Error: PCB is not suspended.\n");
        return -1;
    }

    // Remove from queue
    if (pcb_remove(pcb_to_resume) != 0) {
        print_err("Error encountered when attempting to remove PCB from queue.\n");
        return -1;
    }

    // Change dispatch state
    pcb_to_resume->disp_state = NOT_SUSPENDED;

    // Reinsert
    pcb_insert(pcb_to_resume);

    return 0;
}

int set_pcb_priority(char *name, int priority) {
    // Check for valid name
    if (pcb_validate_name(name) != 0) {
        return -1;
    }

    // Ensure PCB exists
    pcb *pcb_to_modify= pcb_find(name);
    if (pcb_to_modify == NULL) {
        print_err("Error: PCB with given name does not exist.\n");
        return -1;
    }

    // Must NOT be a system process
    if (pcb_to_modify->class == SYSTEM) {
        print_err("Error: A system process cannot be modified.\n");
        return -1;
    }

    // Validate priority
    if (priority < 0 || priority > 9) {
        print_err("Error: PCB priority must be between 0-9.\n");
        return -1;
    }

    // Remove pcb from queue
    if (pcb_remove(pcb_to_modify) != 0) {
        print_err("Error encountered when attempting to remove PCB from queue.\n");
        return -1;
    }

    // Update priority
    pcb_to_modify->priority = priority;

    // Reinsert
    pcb_insert(pcb_to_modify);

    return 0;
}

void show_pcb(char* name)
{
    pcb* found_pcb = pcb_find(name);
    if (found_pcb)
    {
        print("PCB found:\n");
        print_pcb(found_pcb);
    }
    else
    {
        print_err("PCB with name was not found.\n");
    }
}

void show_ready(void)
{
    pcb_print_queues(1, 1, 0, 0);
}

void show_blocked(void)
{
    pcb_print_queues(0, 0, 1, 1);
}

void show_all(void)
{
    pcb_print_queues(1, 1, 1, 1);
}

void pcb_print_queues(int print_R, int print_RS, int print_B, int print_BS)
{
    struct pcb *current;
    //Ready Queue
    pcb *cur = ready_head;
    if (print_R == 1)
    {
        print("All "); print_col("Ready", GREEN); print(" PCBs: \n");
        if (cur == NULL) {
            print("  None\n");
        }
        while(cur!=NULL){
            current = cur;
            print_pcb(current);
            cur = cur->next;
        }
        print("\n");
    }
    //Ready Suspended Queue
    cur = ready_sus_head;
    if (print_RS == 1)
    {
        print("All "); print_col("Ready-Suspended", YELLOW); print(" PCBs: \n");
        if (cur == NULL) {
            print("  None\n");
        }
        while(cur!=NULL){
            current = cur;
            print_pcb(current);
            cur = cur->next;
        }
        print("\n");
    }
    //Blocked Queue
    cur = blocked_head;
    if (print_B)
    {
        print("All "); print_col("Blocked", ORANGE); print(" PCBs: \n");
        if (cur == NULL) {
            print("  None\n");
        }
        while(cur!=NULL){
            current = cur;
            print_pcb(current);
            cur = cur->next;
        }
        print("\n");
    }
    //Blocked Suspened Queue
    cur = blocked_sus_head;
    if (print_BS)
    {
        print("All "); print_col("Blocked-Suspended", RED); print(" PCBs: \n");
        if (cur == NULL) {
            print("  None\n");
        }
        while(cur!=NULL){
            current = cur;
            print_pcb(current);
            cur = cur->next;
        }
        print("\n");
    }
}

void print_pcb(pcb* currpcb)
{
    print("  Name: "); print(currpcb->name); print("\n");
    
    print("    Class: "); print(get_class(currpcb->class)); print("\n");
    print("    Priority: "); print(itoa(currpcb->priority)); print("\n");
    print("    Execution Status: "); print(get_exec_state(currpcb->ex_state)); print("\n");
    print("    Suspend Status: "); print(get_disp_state(currpcb->disp_state)); print("\n");
}

int pcb_validate_name(char *name) {
    // Check for valid name
    if (strlen(name) > 8) {
        print_err("Error: A PCB name must not exceed 8 characters.\n");
        return -1;
    }

    return 0;
}

void yield(void)
{
    // print_col("Yielding the CPU to Processes...\n", YELLOW);
    sys_req(IDLE);
}

// R3/R4 functions

pcb* create_pcb_no_insert(char *name, char *class, int priority) {
    // Ensure PCB does not already exist
    if (pcb_find(name) != NULL) {
        //print_err("Error: PCB with given name already exists.\n");
        return NULL;
    }

    // Ensure name is valid
    if (pcb_validate_name(name) != 0) {
        return NULL;
    }
    
    // Process class
    int class_enum;
    if (strcmp(class, "user") == 0 ) {
        class_enum = USER;
    }
    else if (strcmp(class, "system") == 0) {
         class_enum = SYSTEM;
    }
    else {
        print_err("Error: PCB class must be either user or system.\n");
        return NULL;
    }

    // Priority must be valid (0-9)
    if (priority < 0 || priority > 9) {
        print_err("Error: PCB priority must be between 0-9.\n");
        return NULL;
    }
    
    pcb* new_pcb = pcb_setup(name, class_enum, priority);
    return new_pcb;
}

// void insert_context(pcb* process, void(*func)(void)){

//     context* contxt = (context* )&process->stack[STACK_SIZE] - sizeof(context);
//     process->stackPtr = contxt;

//     contxt->DS = 0x10;
//     contxt->ES = 0x10;
//     contxt->FS = 0x10;
//     contxt->GS = 0x10;
//     contxt->SS = 0x10;

//     contxt->EDI = 0;
//     contxt->ESI = 0;
//     contxt->EBP = (int)&process->stack[0];
//   //  contxt->ESP = (int)&process->stack[1024];
//     contxt->EBX = 0;
//     contxt->EDX = 0;
//     contxt->ECX = 0;
//     contxt->EAX = 0;

//     contxt->EIP = (int)func;
//     contxt->CS = 0x08;
//     contxt->EFLAGS = 0x0202;
// }

void insert_context(pcb* process, void(*func)(void)){
    //sets the context to the top of the stack
    context* contxt = (context* )&process->stack[STACK_SIZE - sizeof(context)];
    process->stackPtr = contxt;

    //assigns each register with the values given
    contxt->DS = 0x10;
    contxt->ES = 0x10;
    contxt->FS = 0x10;
    contxt->GS = 0x10;
    contxt->SS = 0x10;

    contxt->EDI = 0;
    contxt->ESI = 0;
    contxt->EBP = (int)&process->stack[0];
  //  contxt->ESP = (int)&process->stack[1024];
    contxt->EBX = 0;
    contxt->EDX = 0;
    contxt->ECX = 0;
    contxt->EAX = 0;

    contxt->EIP = (int)func;
    contxt->CS = 0x08;
    contxt->EFLAGS = 0x0202;
}

void load_r3(int disp_state){
    if(disp_state != SUSPENDED && disp_state != NOT_SUSPENDED){
        print_err("Wrong display state. Please enter 'suspended' or 'ready' for the display state.\n");
        return;
    }
    //counter to count the number of successfully created pcbs
    int counter = 5;
    //creates a pcb for each process then assigns the context and then inserts into queue
    pcb* p = create_pcb_no_insert("process1", "user", 1);
    if(p!=NULL){
        insert_context(p, proc1);
        if(disp_state == SUSPENDED){
            p->disp_state = SUSPENDED;
        }
        pcb_insert(p);
    }
    else if(p==NULL){
        print_err("process1 is already created! Cannot create a process with the same name!\n");
        counter--;
    }
    p = create_pcb_no_insert("process2", "user", 2);
    if(p!=NULL){
        insert_context(p, proc2);
        if(disp_state == SUSPENDED){
            p->disp_state = SUSPENDED;
        }
        pcb_insert(p);
    }
    else if(p==NULL){
        print_err("process2 is already created! Cannot create a process with the same name!\n");
        counter--;
    }
    p = create_pcb_no_insert("process3", "user", 3);
    if(p!=NULL){
        insert_context(p, proc3);
        if(disp_state == SUSPENDED){
            p->disp_state = SUSPENDED;
        }
        pcb_insert(p);
    }
    else if(p==NULL){
        print_err("process3 is already created! Cannot create a process with the same name!\n");
        counter--;
    }
    p = create_pcb_no_insert("process4", "user", 4);
    if(p!=NULL){
        insert_context(p, proc4);
        if(disp_state == SUSPENDED){
            p->disp_state = SUSPENDED;
        }
        pcb_insert(p);
    }
    else if(p==NULL){
        print_err("process4 is already created! Cannot create a process with the same name!\n");
        counter--;
    }
    p = create_pcb_no_insert("process5", "user", 5);
    if(p!=NULL){
        insert_context(p, proc5);
        if(disp_state == SUSPENDED){
            p->disp_state = SUSPENDED;
        }
        pcb_insert(p);
    }
    else if(p==NULL){
        print_err("process5 is already created! Cannot create a process with the same name!\n");
        counter--;
    }
    char message [] = "0 out of 5 processes created sucessfully\n";
    message[0] = counter + '0';
    print(message);
}

void load_process(char* name){
    pcb* p;
    //creates the process based on the user input
    if(strcmp(name,"process1")==0){
        p = create_pcb_no_insert(name, "user", 1);
        if(p==NULL){
            print_err("Error: Process with given name already exists.\n");
            return;
        }
        insert_context(p,proc1);
        pcb_insert(p);
        print_suc("Process successfully created!\n");
    }
    else if(strcmp(name,"process2")==0){
        p = create_pcb_no_insert(name, "user", 2);
        if(p==NULL){
            print_err("Error: Process with given name already exists.\n");
            return;
        }
        insert_context(p,proc2);
        pcb_insert(p);
        print_suc("Process successfully created!\n");    
    }
    else if(strcmp(name,"process3")==0){
        p = create_pcb_no_insert(name, "user", 3);
        if(p==NULL){
            print_err("Error: Process with given name already exists.\n");
            return;
        }
        insert_context(p,proc3);
        pcb_insert(p);
        print_suc("Process successfully created!\n");
    }
    else if(strcmp(name,"process4")==0){
        p = create_pcb_no_insert(name, "user", 4);
        if(p==NULL){
            print_err("Error: Process with given name already exists.\n");
            return;
        }
        insert_context(p,proc4);
        pcb_insert(p);
        print_suc("Process successfully created!\n");
    }
    else if(strcmp(name,"process5")==0){
        p = create_pcb_no_insert(name, "user", 5);
        if(p==NULL){
            print_err("Error: Process with given name already exists.\n");
            return;
        }
        insert_context(p,proc5);
        pcb_insert(p);
        print_suc("Process successfully created!\n");
    }
    else{  
        //if the user input doesn't match a process name, print an error
        print_err("No process by that name. Please enter 'process1' or 'process2' etc. up to 'process5'.\n");
    }
}

void resume_all(void){
    //iterates through the ready suspended queue
    struct pcb* cur_pcb = ready_sus_head;
    struct pcb* next_pcb;
    while(cur_pcb != NULL){
        next_pcb = cur_pcb->next;
        resume_pcb(cur_pcb->name);
        cur_pcb = next_pcb;
    } 

    //iterates through the blocked suspended queue
    cur_pcb = blocked_sus_head;
    while(cur_pcb != NULL){
        next_pcb = cur_pcb->next;
        resume_pcb(cur_pcb->name);
        cur_pcb = next_pcb;
    }
    print_suc("Processes successfully resumed!\n");
}

int set_alarm(char time[16], char* message)
{
    // PARAM CHECKING ---
    // checking to make sure the correct delimiter is used
    
    int correct_delim=0;
    if(time[2]==':'){
            correct_delim++;
        }
    if(time[5]==':'){
            correct_delim++;
        }
    if(correct_delim!=2){
        print_err("Invalid entry for your delimiter. Please use ':' to break up the time and make sure it's in the correct spot. It should be HH:MM:SS\n");
        return -1; 
    }
    int second, minute, hour;
    char *tok = strtok(time, ":");
    hour = atoi(tok);
    tok = strtok(NULL, ":");
    minute = atoi(tok);
    tok = strtok(NULL, "/");
    second = atoi(tok);

    // Checking to see if the second, minute, and hour is valid
    if (second < 0 | second > 60)
    {
        print_err("Invalid entry for second. Please enter between the range of 0-60\n");
        return -1;
    }
    if (minute < 0 || minute > 60)
    {
        print_err("Invalid entry for minute. Please enter between the range of 0-60\n");
        return -1;
    }
    if (hour < 0 || hour > 23)
    {
        print_err("Invalid entry for hour. Please enter between the range of 1-24\n");
        return -1;
    }
    // PARAM CHECKING END -----

    pcb* alarmPCB;
    char* processName = "alarm";
        
    alarm_add(message, hour, minute, second);
    if(pcb_find(processName) == NULL)
    {
        alarmPCB = create_pcb_no_insert(processName, "system", 1);
        if(alarmPCB == NULL){
            //add 1 to alarm
            print_err("alarm allocation failed");
            return -1;
        }
        insert_context(alarmPCB, running_alarm);

        pcb_insert(alarmPCB);
    }

    return 0;
}

// R5 Functions

int alloc_mem(int size) {
    //allocates memory and returns the address
    void *addr = allocate_memory(size);

    if(addr==NULL){
        print_err("Unable to allocate address.\n");
        return -1;
    }
    else{
        //convert to hexadecimal
        print_suc("Block successfully allocated!\n");
        print_col("Address of Block: ", YELLOW);
        print_col(itoh((int) addr), YELLOW);
        print_char('\n');
        return 0;
    }
}

int free_mem(char *block_addr) {
    int hexToInt = htoi(block_addr);
    if (hexToInt == -1)
    {
        print_err("A hex address is expected. Ex. 0x34FC\n");
        return -1;
    }
    void *hexConversion = (void *)hexToInt;
    //print(itoa(hexConversion));
    int flag = free_memory(hexConversion);
    if(flag==-1){
        print_err("Freeing failed!\n");
        return -1;
    }
    print_suc("Freeing successful!\n");
    return 0;
}

// The flag determines if the function prints allocated or free mcbs. 0=free, 1= allocated
void print_mcbs(int flag)
{
    mcb* cur = mcbHead;
    while (cur != NULL)
    {
        if (cur->flag == flag)
        {
            print_mcb(cur);
        }
        cur = cur->next;
    }
}
void print_mcb(mcb* curr_mcb)
{
    print_col("Start Address:", YELLOW);
    print_col(itoh((uintptr_t)curr_mcb->startAdd), YELLOW);

    print_col("\nSize of Block: ", YELLOW);
    print_col(itoa(curr_mcb->size), YELLOW);
    print("\n\n");

}
