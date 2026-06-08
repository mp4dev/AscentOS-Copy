#include "mpx/devicedriver.h"
#include <mpx/iocb.h>
#include <mpx/device.h>
#include "mpx/devicedriver.h"
void serial_interrupt(void)
{
    //Disable interrupts
    cli();
    //Get the correct DCB by iterating through to see which has the event flag triggered
    device dev = 0;
    dcb* d;
    for(int i = 0; i<4; i++){
        d = dcb_array[i];
        if(d != NULL && d->event_flag == 0){
            //If the dcb is open then clear the interrupt and return
            if(d->status == 0){
                d->event_flag = 0;
                return;
            }
            dev = d->dev;
            break;
        }
    }
    //Read the Interrupt ID Register to determine the interrupt
    char interrupt = inb(dev+2);
    //mask every bit except bit 1 and 2
    interrupt = interrupt & 0x06;
    //Pass to correct one depending on interrupt
    //Modern Status Interrupt
    if(interrupt == 0x00){
        inb(dev+6);
    }
    //Output Interrupt
    if(interrupt == 0x04){
        serial_input_interrupt(d);
    }
    //Input Interrupt
    if(interrupt == 0x02){
        serial_output_interrupt(d);
    }
    //Line Status Interrupt
    if(interrupt == 0x06){
        inb(dev+5);
    }
    //Issue EOI to PIC
    outb(0x20,0x20);
    //Reenable interrupts
    sti();
}

void serial_input_interrupt(dcb* dcb)
{
    device dev = dcb->dev;
    char read_char = inb(dev);
    if (dcb->status != 1)
    {
        if (dcb->curr_iocb->buffer_size == dcb->counter)
        {
            return;
        }
        store_char(dcb->ring_buf, dcb->ring_size, read_char);
        return;
    }
    else if (dcb->status == 1)
    {
        store_char(dcb->curr_iocb->buffer, dcb->curr_iocb->buffer_size, read_char);
    }

    if (dcb->event_flag == 0 && read_char != '\n')
    {
        return;
    }

    dcb->status = 0;
    dcb->event_flag = 1;
    //return dcb->counter;

}

void serial_output_interrupt(dcb* dcb)
{
    if(dcb->curr_iocb->operation != 3){
        return;
    }
    device dev = dcb->dev;
    iocb* iocbtemp = dcb->curr_iocb;
    if(iocbtemp->buffer_size != dcb->counter){
        outb(dev, iocbtemp->buffer[dcb->counter]);
        dcb->counter++;
    }
    if(iocbtemp->buffer_size == dcb->counter){
        dcb->curr_op = 1;
        dcb->event_flag = 1;
        dcb->counter = 0;
        dcb->status = 0;
    }
}

int store_char(char* buffer, int len, char single_char)
{
    (void) buffer;
    (void) len;
    (void) single_char;
    
    for (int i = 0; i < len; i++)
    {
        if (buffer[i] == 0) // Numeric 0 is considered null character
        {
            buffer[i] = single_char;
            return 0;
        }
    }
    return -1;
}
