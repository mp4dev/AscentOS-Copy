#include <mpx/print.h>
#include <mpx/color.h>
#include <sys_req.h>
#include <string.h>

void print(char *str) {
    sys_req(WRITE, COM1, str, strlen(str));
}

void print_err(char *str) {
    set_color(RED);
    sys_req(WRITE, COM1, str, strlen(str));
    reset_color();
}

void print_suc(char *str) {
    set_color(GREEN);
    sys_req(WRITE, COM1, str, strlen(str));
    reset_color();
}

void print_char(char c) {
    // Since sys_req expects an address, must use address of character, not character itself
    sys_req(WRITE, COM1, &c, sizeof(c));
}

void print_ws(int count) {
    for (int i = 0; i < count; i++) {
        print_char(' ');
    }
}

void print_hr(void) {
    for (int i = 0; i < 152; i++) {
        print_char('-');
    }
    print_char('\n');
}

void print_col(char *str, char *color) {
    set_color(color);
    print(str);
    reset_color();
}

// void printf(const char *fmt_str, ...) {}
