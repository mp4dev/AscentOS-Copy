#ifndef MPX_SERIAL_H
#define MPX_SERIAL_H

#include <stddef.h>
#include <mpx/device.h>

/**
 @file mpx/serial.h
 @brief Kernel functions and constants for handling serial I/O
*/

struct history;

/**
 Initializes devices for user input and output
 @param device A serial port to initialize (COM1, COM2, COM3, or COM4)
 @return 0 on success, non-zero on failure
*/
int serial_init(device dev);

char* strcpy(char destination[], char source[]);

/**
 Writes a buffer to a serial port
 @param device The serial port to output to
 @param buffer A pointer to an array of characters to output
 @param len The number of bytes to write
 @return The number of bytes written
*/
int serial_out(device dev, const char *buffer, size_t len);

/**
 Reads a string from a serial port
 @param device The serial port to read data from
 @param buffer A buffer to write data into as it is read from the serial port
 @param count The maximum number of bytes to read
 @return The number of bytes read on success, a negative number on failure
*/   		   	
int serial_poll(device dev, char *buffer, size_t len);

/**
 Processes Delete key presses and backspaces
 @param buffer A buffer to display deleted data to teh terminal
 @param bytesRead Used for indexing the buffer index when shifting deleted data
 @param cursor The position of the cursor relative to the movement being done by mpx user
 @param is_DEL 1(True) or 0(False) value that processes serial input if the delete key is pressed or backspace
*/   	
void delete_handler(char* buffer, size_t *bytesRead, size_t* cursor, int* is_DEL);

/**
 Processes whether an arrow key or the delete key is pressed
 @param buffer A buffer adjust the indexing of the cursor
 @param cursor The position of the cursor relative to arrow key presses made by user
 @param bytesRead The number of bytes being read by polling function
 @param device The serial port to take in user input then process whether an arrow key was pressed
 @param starting_byte The first byte read by the serial port
*/   	
void arrow_handler(char* buffer, size_t* cursor, size_t* bytesRead, device dev, char starting_byte);

/**
 * Stores the content from one string into another
 * @param desination where the string is going to be stored
 * @param source the string you want to store
 * @return a pointer to a character array with copied contents from source
 */
char* strcpy(char destination[], char source[]);

#endif
