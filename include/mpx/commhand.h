/**
 @file commhand.h
 @brief Handles commands by calling corresponding function.
*/

/**
 * @brief The command handler 
 * 
 * Handles commands entered by user and calls corresponding function.
 */
void commhand(void);

/**
 * @brief Helper function for flashy startup
 * 
 * Prints our custom ASCII logo, along with a greeting to the OS
 */
void startup_sequence(void);

/**
 * @brief Returns 1 if the parameter count is in expected range. Returns 0 if not in expected range.
 * 
 * min and max are both inclusive.
 */
int count_expected_range(int param_count, int min, int max);
/**
 * @brief Returns 1 if the parameter count equals the amount. Returns 0 if not in expected range,
 */
int count_expected(int param_count, int expected_count);
/**
 * @brief Prints an error to the console saying the parameter count was out of range with the command name provided.
 * 
 * min and max are the expected parameter counts inclusive, not expected argument counts.
 */
void print_param_bounds_error_range(int param_count, int expected_min, int expected_max, char* command_name);

/**
 * @brief Prints an error to the console saying the parameter count was out of range with the command name provided.
 * 
 * expected_count are the expected parameter counts inclusive, not expected argument counts. 
 * Example: If no parameters are needed, expected_count should be 0.
 */
void print_param_bounds_error(int param_count, int expected_count, char* command_name);
