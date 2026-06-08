#ifndef MPX_STDLIB_H
#define MPX_STDLIB_H

/**
 @file stdlib.h
 @brief A subset of standard C library functions.
*/

/**
 Convert an ASCII string to an integer
 @param s A NUL-terminated string
 @return The value of the string converted to an integer
*/
int atoi(const char *s);

/**
 * Convert an integer to an string
 * @param int An integer
 * @return The pointer to the resulting static string
*/
char *itoa(int value);

/**
 * Converts a BCD value to integer.
 * @param char A char
 * @return integer containing converted number
 */
int btoi(char number);

/**
 * Converts an integer to BCD.
 * @param int An integer
 * @return char containing converted number
 */
char itob(int number);

/**
 * Converts an integer to a hex string.
 * @param dec_num An integer
 * @return a hexadecimal string equivalent to the provided integer
 */
char *itoh(int dec_num);

/**
 * Converts a hex string to an integer
 * @param hex_num A hexidecimal number string
 * @return an integer equivalent to the provided hexadecimal number
 */
int htoi(char *hex_num);

/**
 * Converts a hex char to an integer
 * @param digit a hexidecimal digit
 * @return an integer equivalent to the provided hexadecimal digit
 */
int int_of_hex(char digit);

/**
 * Converts a number into a single hex 
 * @param num a decimal number 0-15
 * @return an hexidecimal digit equivalent to the provided number
 */
char hex_of_int(int num);

#endif
