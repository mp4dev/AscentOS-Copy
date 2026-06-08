#pragma once
#ifndef IOCB_H
#define IOCB_H

#include <mpx/pcb.h>
#include <mpx/dcb.h>
#include <mpx/device.h>
#include <mpx/interrupts.h>
#include <mpx/mcb.h>
#include <mpx/io.h>
#include <mpx/mcb.h>
struct dcb;
#define DEVICECOUNT 4
extern struct dcb* dcb_array[DEVICECOUNT];



typedef struct iocb{
    pcb *p;
    struct dcb *d;
    int operation;
    char* buffer;
    int buffer_size;
    struct iocb* next;
}iocb;

struct dcb *find_dcb(device dev);
int dcbArrLoc(device dev);
int serial_open(device dev, int speed);
int serial_close(device dev);

int serial_read(device dev, char *buf, size_t len);

int serial_write(device dev, char *buf, size_t len);
#endif
