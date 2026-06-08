#ifndef MPX_COLORS_H
#define MPX_COLORS_H

/**
 @file color.h
 @brief Contains funtions to modify terminal text color
*/

// Each ASCII color code is 6 bytes, bar COLOR_RESET which is 5
#define TEXT_COLOR_RESET  "\x1b[0m"
#define GEN_RESET         "\033[0m"
#define BG_RESET          "\033[49m"

/**
 *  RGB 256-color format: 
 *  \x1b[38;2;R;G;Bm - foreground
 *  \x1b[48;2;R;G;Bm - background
*/

// Basic text colors
#define BLACK       "\x1b[30m"
#define RED         "\x1b[31m"
#define GREEN       "\x1b[32m"
#define YELLOW      "\x1b[33m"
#define BLUE        "\x1b[34m"
#define MAGENTA     "\x1b[35m"
#define CYAN        "\x1b[36m"
#define WHITE       "\x1b[37m"

// RGB Text colors
#define ORANGE "\x1b[38;2;255;121;0m"

// Basic background colors
#define BG_BLACK    "\x1b[40m"
#define BG_RED      "\x1b[41m"
#define BG_GREEN    "\x1b[42m"
#define BG_YELLOW   "\x1b[43m"
#define BG_BLUE     "\x1b[44m"
#define BG_MAGENTA  "\x1b[45m"
#define BG_CYAN     "\x1b[46m"
#define BG_WHITE    "\x1b[47m"

/**
 * @brief Sets text color
 * 
 * Sets the current terminal text color to the provided ASCII color code
 * @param color An ASCII color code
 */
void set_color(char* color);

/**
 * @brief Resets text color
 * 
 * Sets the current text color back to terminal default
 */
void reset_color(void);

#endif
