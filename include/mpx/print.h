#ifndef MPX_PRINT_H
#define MPX_PRINT_H

/**
 * @file print.h
 * @brief Contains functions for printing text to the terminal
 */

/**
 * Prints the given string to the terminal
 * @param str A string
 */
void print(char *str);

/**
 * Prints the given string to the terminal in red
 * @param str A string
 */
void print_err(char *str);

/**
 * Prints the given string to the terminal in green
 * @param str A string
 */
void print_suc(char *str);

/**
 * Prints the provided character
 * @param c A character
 */
void print_char(char c);

/**
 * Prints whitespace
 * @param count Number of spaces to print
 */
void print_ws(int count);

/**
 * Prints a horizontal rule
 */
void print_hr(void);

/**
 * Prints text with given color
 * @param str A string
 * @param color A color constant from color.h
 */
void print_col(char *str, char *color);

/**
 * TODO: implement printf() function?
 * @param fmt_str A formatted string
 */
void printf(const char *fmt_str, ...);

#endif
