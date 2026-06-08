#include <mpx/dcb.h>

void serial_interrupt(void);

void serial_input_interrupt(dcb *dcb);

void serial_output_interrupt(dcb *dcb);

int store_char(char *buffer, int len, char single_char);
