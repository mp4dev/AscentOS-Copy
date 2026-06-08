#pragma once
#include <mpx/iocb.h>
#include <mpx/device.h>
struct iocb;

typedef struct dcb {

    // @brief 0 - Not in use. 1 - In Use
    int status;
    // @brief 2 - Read. 3 - Write 
    int curr_op;
    // @brief 1 if exists event needing to be handled
    int event_flag;
    
    char *ring_buf;
    int ring_read;
    int ring_size;
    int ring_write;
    int ring_chars_unread;
    struct iocb* head;
    struct iocb* tail;
    struct iocb* curr_iocb;
    device dev;
    int port_base;
    int counter;

} dcb;
 
char ring_read(dcb* device);
void ring_write(dcb* device, char input);
int loop_ring(int place, int length);
void enqueue_iocb(dcb *d, struct iocb *new_iocb);
struct iocb *dequeue_iocb(dcb *d);
int has_unread_char(dcb *device);
