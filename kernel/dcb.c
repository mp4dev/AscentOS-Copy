#include <mpx/dcb.h>
#include <stddef.h>
#include "mpx/dcb.h"

int has_unread_char(dcb* device)
{
    return device->ring_chars_unread > 0;
}
char ring_read(dcb* device)
{
    // Failsafe. You should use has_unread_char during your while loops
    if (!has_unread_char(device))
    {
        return 0;
    }
    int read = device->ring_read;
    int length = device->ring_size;
    
    char readChar = device->ring_buf[read];
    read++;
    read = loop_ring(read, length);

    device->ring_read = read;
    device->ring_chars_unread--;

    return readChar;
}

void ring_write(dcb* device, char input)
{
    
    int write = device->ring_write;
    int length = device->ring_size;

   device->ring_buf[write] = input;

    write++;
    write = loop_ring(write, length); // In case
    device->ring_write = write;

    // If full, overwrite oldest byte and move read forward.
    if (device->ring_chars_unread == device->ring_size)
    {
        device->ring_read = write;
    }

    else
    {
        device->ring_chars_unread++;
    }

}

int loop_ring(int place, int length)
{
    return place % length;
}
void enqueue_iocb(dcb *d, struct iocb *new_iocb) {
    new_iocb->next = NULL;

    if (d->tail == NULL) {
        d->head = new_iocb;
        d->tail = new_iocb;
    } else {
        d->tail->next = new_iocb;
        d->tail = new_iocb;
    }
}

iocb* dequeue_iocb(dcb *d) {
    if (d->head == NULL) {
        return NULL; 
    }

    iocb *item = d->head;
    d->head = d->head->next;

    if (d->head == NULL) {
        d->tail = NULL; 
    }

    item->next = NULL;
    return item;
}
