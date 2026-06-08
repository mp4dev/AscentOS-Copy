#include <mpx/commhand.h>
#include <mpx/commands.h>
#include <../include/stdlib.h> // some compilers assumed functions declared in standard stdlib.h if not delcared explicitly
#include <sys_req.h>
#include <string.h>
#include <mpx/color.h>
#include <mpx/print.h>
#include <mpx/alarm.h>
#include <mpx/sys_call.h>

/**
 * @def GROUP_NAME
 * @brief Name of CS 450 group
 */
#define GROUP_NAME "AscentOS"

/**
 * @def MAX_ARG_COUNT
 * @brief Maximum number of arguments read by commhand.
 */
#define MAX_ARG_COUNT 8

void startup_sequence(void) {
    char logo[] =   "\n"
                    "                             @@\n"
                    "                            @@@@\n"
                    "                            @@@@\n"
                    "                           @@@@@@\n"
                    "                          @@@@@@@@\n"
                    "                          @@@@@@@@\n"
                    "                         @@@@@@@@@@\n"
                    "                        @@@@@@@@@@@@\n"
                    "                        @@@@@@@@@@@@\n"
                    "                       @@@@@@@@@@@@@@\n"
                    "                      @@@@@@@@@@@@@@@\n"
                    "                      @@@@@@@@@@@@@@@@           **\n"
                    "                     @@@@@@@@@@@@@@@@@          +***\n"
                    "                     @@@@@@@@@@@@@@@@@@         +***\n"
                    "                    @@@@@@@@@@@@@@@@@@@@       ++****\n"
                    "                    @@@@@@@@@@@@@@@@@@@@      +++*****\n"
                    "                   @@@@@@@@@@@@@@@@@@@@@@     +++****o\n"
                    "                  @@@@@@@@@@@@@@@@@@@@@@@@   ++++****oo\n"
                    "                  @@@@@@@@@@@@@@@@@@@@@@@@  +++++****oo#\n"
                    "                 @@@@@@@@@@@@@@@@@@@@@@@@@@ +++++****oo#\n"
                    "                @@@@@@@@@@@@@@@@@@@@@@@@@@@++++++****oo##\n"
                    "                @@@@@@@@@@@@@@@@@@@@@@@@@@@++++++****oo##\n"
                    "               @@@@@@@@@@@@@@@@@@@@@@@@@@@=++++++****oo###\n"
                    "              @@@@@@@@@@@@@@@@@@@@@@@@@@@@=++++++****oo###\n"
                    "              @@@@@@@@@@@@@@@@@@@@@@@@@@@==++++++****oo####\n"
                    "             @@@@@@@@@@@@@@@@@@@@@@@@:@@===++++++****oo#####\n"
                    "            @@@@@@@@@@@@@@@@@@@@@@@@:::====++++++****oo#####\n"
                    "            @@@@@@@@@@@@@@@@@@@@@@@:::::===++++++****oo#####%\n"
                    "           @@@@@@@@@@@@@@@@@@@@@@@:::::::==++++++****oo#####%\n"
                    "           @@@@@@@@@@@@@@@@@@@@@@@:::::::==++++++****oo#####%%                                                        @      @@@@@@@@@      @@@@@@@@   \n"
                    "          @@@@@@@@@@@@@@@@@@@@@@@:::::::::=++++++****oo#####%%                                                      @@@@    @@@@@@@@@@@@   @@@@@@@@@@@ \n"
                    "          @@@@@@@@@@@@@@@@@@@@@@:::::::::::++++++****oo#####%%%      @@@@@@@      @@@@@@      @@@@@    @@@@@@@@@   @@@@@@@ @@@@@    @@@@@ @@@@    @@@@ \n"
                    "         @@@@@@@@@@@@@@@@@@@@@@@:::::::::::++++++****oo#####%%%%    @@@@@@@@@   @@@@@@@@@@  @@@@@@@@@  @@@@@@@@@@  @@@@@@@@@@@       @@@@ @@@@@@@@@@   \n"
                    "        @@@@@@@@@@@@@@@@@@@@@@@:::::::::::::+++++****oo#####%%%%   @@@@@@@     @@@@   @@@@@ @@@   @@@@ @@@@   @@@@  @@@@  @@@@       @@@@@  @@@@@@@@@@ \n"
                    "        @@@@@@@@@@@@@@@@@@@@@@:::::::::::::::++++****oo#####%%%%%   @@@@@@@@@@ @@@@        @@@@@@@@@@@ @@@@   @@@@  @@@@  @@@@       @@@@  @@   @@@@@@@\n"
                    "       @@@@@@@@@@@@@@@@@@@@@@@:::::::::::::::++++****oo#####%%%%%      @@@@@@@ @@@@   @@@@@ @@@        @@@@   @@@@  @@@@   @@@@@    @@@@@ @@@@    @@@@@\n"
                    "      @@@@@@@@@@@@@@@@@@@@@@@:::::::::::::::::+++****oo#####%%%%%% @@@@@@@@@@@  @@@@@@@@@@  @@@@@@@@@@ @@@@   @@@@  @@@@@@  @@@@@@@@@@@@   @@@@@@@@@@@ \n"
                    "      @@@@@@@@@@@@@@@@@@@@@@@:::::::::::::::::+++****oo#####%%%%%%   @@@@@@@     @@@@@@@     @@@@@@@    @@@   @@@    @@@@     @@@@@@@       @@@@@@@@   \n"
                    "     @@@@@@@@@@@@@@@@@@@@@@@:::::::::::::::::::++****oo#####%%%%%%@\n"
                    "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@------=====++++++****oo#####%%%%%%@@\n"
                    "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@------=====++++++****oo#####%%%%%%@@\n"
                    "   @@@@@@@@@@@@@@@@@@@@@@@@@@@@-------=====++++++****oo#####%%%%%%@@@\n"
                    "  @@@@@@@@@@@@@@@@@@@@@@@@@@@@--------=====++++++****oo#####%%%%%%@@@@\n"
                    "  @@@@@@@@@@@@@@@@@@@@@@@@@@@@--------=====++++++****oo#####%%%%%%@@@@\n"
                    " @@@@@@@@@@@@@@@@@@@@@@@@@@@@---------=====++++++****oo#####%%%%%%@@@@@\n"
                    " @@@@@@@@@@@@@@@@@@@@@@@@@@@@---------=====++++++****oo#####%%%%%%@@@@@\n"
                    "@@@@@@@@@@@@@@@@@@@@@@@@@@@@----------=====++++++****oo#####%%%%%%@@@@@@\n"
                    "\n";
    char greeting[] = "Welcome to "GROUP_NAME"! Type \"help\" to get started.\n";
    
    set_color(BLUE);
    print_hr();

    print_col(logo, CYAN);

    set_color(BLUE);
    print_hr(); 

    set_color(CYAN);
    print_ws(52);
    print(greeting);

    set_color(BLUE);
    print_hr();
}

void mini_startup(void) {
    set_color(BLUE);
    for (int i = 0; i < 20; i++) {
        print_char('~');
    } print_char('\n');

    print_ws(5);
    print_col("Welcome to\n\n", CYAN);
    print_col("    /\\\n", CYAN);
    print_col("   /  \\/\\\n", CYAN);
    print_col("  /    \\ \\\n", CYAN);
    print_col(" /  /\\  \\ \\ scentOS\n", CYAN);
    print_col("/________\\-'\n", CYAN);
    
    print_col("\n Type \"help\" to get\n", CYAN);
    print_ws(5);
    print_col("started.\n", CYAN);

    set_color(BLUE);
    for (int i = 0; i < 20; i++) {
        print_char('~');
    }
    print_char('\n');
}

void commhand(void)
{
    mini_startup();
    for (;;) {
        set_color(TEXT_COLOR_RESET);
        yield(); // AKA sys_req(IDLE)

        // show command prompt
        print_col(GROUP_NAME" >", CYAN);

        // read user input
        char buf[100] = { 0 };
        int nread = sys_req(READ, COM1, buf, sizeof(buf));

        // If no input, skip to new prompt
        if(nread == 0 || buf[0] == '\0' || buf[0] == '\n' || buf[0] == '\r') {
            continue;
        }

        // tokenize arguments
        char *args[MAX_ARG_COUNT];
        int arg_count = 0;

        char *curr_arg = strtok(buf, " ");
        // while arg still exists, and max count not reached
        while (curr_arg != NULL && arg_count < MAX_ARG_COUNT) {
            // add token to token array
            args[arg_count++] = curr_arg;
            curr_arg = strtok(NULL, " "); // get next arg
        }

        char *command = args[0]; // command should be first string in input
        char **params = &args[1]; // Gives the parameters of the input, without the command. params[0] is the first param
        int param_count = arg_count - 1;

        int expected_count = -1; // initialized to -1 to force people to assign it.
        // Based on command, call correct function
        if (strcmp(command, "version") == 0 || strcmp(command, "v") == 0) {
            expected_count = 0;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "version");
            }
            else {
                version();
            }
        }
        else if (strcmp(command, "help") == 0) {
            if (!count_expected_range(param_count, 0, 1))
            {
                print_param_bounds_error_range(param_count, 0, 1, "help");
            }
            else if (param_count == 0 ) {
                help();
            }
            else if(param_count == 1) {
                help_extra(params[0]);
            }
        }
        else if (strcmp(command, "shutdown") == 0 || strcmp(command, "sh") == 0) {
            expected_count = 0;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "shutdown");
            }
            else {
                if (shutdown() == 0) { // Shutdown confirmed
                    print_col("Shutdown confirmed. Exiting "GROUP_NAME"...\n", YELLOW);
                    // Delete any pcbs in ready queue so that system can shutdown
                    while (ready_head != NULL) {
                        pop(&ready_head);
                    }
                    sys_req(EXIT); // exit out of commhand
                }

                // Otherwise, shutdown halted
                print_col("Shutdown halted. Returning control to commhand...\n", YELLOW);
            }
        }
        else if(strcmp(command, "clear") == 0) {
            expected_count = 0;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "clear");
            }
            else {
                clear();
            }
        }
        else if (strcmp(command, "get_date") == 0) {
            expected_count = 0;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "get_date");
            }
            else {
                print_col(get_date(), YELLOW);
            }
        }
        else if (strcmp(command, "set_date") == 0) {
            expected_count = 1;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "set_date");
            }
            else
            {
                char *date = params[0];
                if (set_date(date) == 0) {
                    print_suc("Date set successfully!\n");
                }
            }
        }
        else if (strcmp(command, "get_time") == 0) {
            expected_count = 0;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "get_time");
            }
            else {
                print_col(get_time(), YELLOW);
            } 
        }
        else if (strcmp(command, "set_time") == 0) {
            // ensure correct # of arguments
            expected_count = 1;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "set_time");
            }
            else {
                char *time = params[0];
                if (set_time(time) == 0) {
                    print_suc("Time set successfully!\n");
                }
            }
        }
        // Disable create_pcb from commhand
        /*
        else if (strcmp(command, "create_pcb") == 0 || strcmp(command, "create") == 0) {
            if (arg_count != 4) {
                print_err("Error: create_pcb takes 3 arguments, you provided: ");
                print_err(itoa(arg_count-1));
                print_err(". Type \"help create_pcb\" for expected format.\n");
            }
            // Ensure atoi doesn't default to zero
            else if (atoi(args[3]) == 0 && strcmp(args[3], "0") != 0) {
                print_err("Error: Please enter an integer for priority.\n");
            }
            else {
                if (create_pcb(args[1], args[2], atoi(args[3])) == 0) {
                    print_suc("PCB created successfully!\n");
                }
            }
        }*/
        else if (strcmp(command, "delete_pcb") == 0 || strcmp(command, "delete") == 0) {
            expected_count = 1;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "delete_pcb");
            }
            else {
                if (delete_pcb(params[0]) == 0) {
                    print_suc("PCB deleted successfully!\n");
                }
            }
        }
        else if (strcmp(command, "block_pcb") == 0 || strcmp(command, "block") == 0) {
            expected_count = 1;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "block_pcb");
            }
            else {
                if (block_pcb(params[0]) == 0) {
                    print_suc("PCB blocked successfully!\n");
                }
            }
        }
        else if (strcmp(command, "unblock_pcb") == 0 || strcmp(command, "unblock") == 0) {
            expected_count = 1;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "unblock_pcb");
            }
            else {
                if (unblock_pcb(params[0]) == 0) {
                    print_suc("PCB unblocked successfully!\n");
                }
            }
        }
        else if (strcmp(command, "suspend_pcb") == 0 || strcmp(command, "suspend") == 0) {
            expected_count = 1;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "suspend_pcb");
            }
            else {
                if (suspend_pcb(params[0]) == 0) {
                    print_suc("PCB suspended successfully!\n");
                }
            }
        }
        else if (strcmp(command, "resume_pcb") == 0 || strcmp(command, "resume") == 0) {
            expected_count = 1;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "resume_pcb");
            }
            else {
                if (resume_pcb(params[0]) == 0) {
                    print_suc("PCB resumed successfully!\n");
                }
            }
        }
        else if (strcmp(command, "set_priority") == 0) {
            expected_count = 2;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "set_priorty");
            }
            // Ensure atoi doesn't default to zero
            else if (atoi(params[1]) == 0 && strcmp(params[1], "0") != 0) {
                print_err("Error: Please enter an integer for priority.\n");
            }
            else {
                if (set_pcb_priority(params[0], atoi(params[0])) == 0) {
                    print_suc("PCB priority set successfully!\n");
                }
            }
        }
        else if (strcmp(command, "show_pcb") == 0 || strcmp(command, "show") == 0) {
            expected_count = 1;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "show_pcb");
            }
            else {
                    show_pcb(params[0]);
            }
        }
        else if (strcmp(command, "show_ready") == 0) {
            expected_count = 0;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "show_ready");
            }
            else{
                show_ready();
            }
        }
        else if (strcmp(command, "show_blocked") == 0) {
            expected_count = 0;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "show_blocked");
            }
            else{
                show_blocked();
            }
        }
        else if (strcmp(command, "show_all") == 0) {
            expected_count = 0;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "show_all");
            }
            else{
                show_all();
            }
        }
        // Disable yield from commhand
        // else if (strcmp(command, "yield") == 0) {
        //     expected_count = 0;
        //     if (!count_expected(param_count, expected_count)) {
        //         print_param_bounds_error(param_count, expected_count, "yield");
        //     }
        //     else {
        //         yield();
        //     }
        // }
        else if (strcmp(command, "load_r3") == 0) {
            expected_count = 1;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "load_r3");
            }
            else{
                    if (strcmp(params[0], "ready") == 0) {
                        load_r3(NOT_SUSPENDED);
                    }
                    else if (strcmp(params[0], "suspended") == 0) {
                        load_r3(SUSPENDED);
                    }
            }
        }
        else if (strcmp(command, "load_process") == 0) {
            expected_count = 1;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "load_process");
            }
            else {
                load_process(params[0]);
            }
        }
        
        else if (strcmp(command, "resume_all") == 0) {
            expected_count = 0;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "resume_all");
            }
            else{
                resume_all();
            }
        }

        else if (strcmp(command, "alarm") == 0) {
            expected_count = 1;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "alarm");
            }
            else{
                char message[ALARM_MSG_LEN];
                print_col("Input a message to accompany alarm: ", YELLOW);
                sys_req(READ, COM1, message, sizeof(message));
                if (set_alarm(params[0], message) == 0) {
                    print_suc("Alarm set successfully!\n");
                }
            }
        }

        // Remove from commhand for R5 pt. 2
        // else if (strcmp(command, "allocate_mem") == 0) {
        //     expected_count = 1;
        //     if (!count_expected(param_count, expected_count)) {
        //         print_param_bounds_error(param_count, expected_count, "allocate_mem");
        //     }
        //     else{
        //         alloc_mem(atoi(params[0]));
        //     }
        // }
        
        // Remove from commhand for R5 pt. 2
        // else if (strcmp(command, "free_mem") == 0) {
        //     expected_count = 1;
        //     if (!count_expected(param_count, expected_count)) {
        //         print_param_bounds_error(param_count, expected_count, "free_mem");
        //     }
        //     else{
        //         //convert to hex?
        //         free_mem(params[0]);
        //     }
        // }

        else if (strcmp(command, "show_allocated") == 0) {
            expected_count = 0;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "show_allocated");
            }
            else{
                print_col("Allocated MCBs:\n\n", YELLOW);
                print_mcbs(1);
            }
        }

        else if (strcmp(command, "show_free") == 0) {
            expected_count = 0;
            if (!count_expected(param_count, expected_count)) {
                print_param_bounds_error(param_count, expected_count, "show_free");
            }
            else{
                print_col("Free MCBs:\n\n", YELLOW);
                print_mcbs(0);
            }
        }
        // // FOR DEBUGGING
        // else if (strcmp(command, "test_itoh") == 0) {
        //     expected_count = 1;
        //     if (!count_expected(param_count, expected_count)) {
        //         print_param_bounds_error(param_count, expected_count, "test_itoh");
        //     }
        //     else{
        //         print(itoh(atoi(params[0])));
        //         print_char('\n');
        //     }
        // }
        else {
            // Default: command not recognized
            print_err("Error: Command not recognized. Type \"help\" to see available commands.\n");
        }
    }
    // Returns 0 if parameter is in range. Returns 1 if not

}


int count_expected_range(int param_count, int min, int max)
    {
        int in_range = ((param_count >= min) && (param_count <= max));
        return in_range;
    }
int count_expected(int param_count, int expected_count)
    {
        int in_range = (param_count == expected_count);
        return in_range;
    }

void print_param_bounds_error_range(int param_count, int expected_min, int expected_max, char* command_name)
{
    print_err("Error: ");
    print_err(command_name);
    print_err(" takes ");
    print_err(itoa(expected_min));
    print_err("-");
    print_err(itoa(expected_max));
    print_err(" arguments, you provided: ");
    print_err(itoa(param_count));
    print_err(". Type \"help ");
    print_err(command_name);
    print_err("\" for expected format.\n");
}

void print_param_bounds_error(int param_count, int expected_count, char* command_name)
{
    print_err("Error: ");
    print_err(command_name);
    print_err(" takes ");
    print_err(itoa(expected_count));
    print_err(" argument(s), you provided: ");
    print_err(itoa(param_count));
    print_err(". Type \"help ");
    print_err(command_name);
    print_err("\" for expected format.\n");
}
