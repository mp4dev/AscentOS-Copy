#include <mpx/color.h>
#include <sys_req.h>
#include "mpx/color.h"
#include <mpx/print.h>

void set_color(char *color) {
    if (!color) return; // if color ptr is null, reutrn
    print(color);
}

void reset_color(void) {
    print(TEXT_COLOR_RESET);
    print(BG_RESET);
}
