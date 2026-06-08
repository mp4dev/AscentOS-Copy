#include <mpx/iocb.h>
#include <mpx/print.h>
#include "sys_req.h"
#include <mpx/devicedriver.h>
#include <memory.h>

extern void serial_isr(void*);

struct dcb* dcb_array[DEVICECOUNT] = {0};

dcb* find_dcb(device dev)
{
    switch(dev)
    {
        case COM1:
            return dcb_array[0];
            break;
        case COM2:
            return dcb_array[1];
            break;
        case COM3:
            return dcb_array[2];
            break;
        case COM4:
            return dcb_array[3];
            break;
        default:
            return NULL;
    }
}
int dcbArrLoc(device dev){
    switch(dev){
        case COM1:
            return 0;
        case COM2:
            return 1;
        case COM3:
            return 2;
        case COM4:
            return 3;
        default:
            return -1;
    }
}


int serial_open(device dev, int speed)
{
    //Check to see if the device is valid
    if(dev!= COM1 && dev!= COM2 && dev!= COM3 && dev!= COM4){
        print_err("Not a valid device!");
        return -1;
    }
    //make sure speed is valid
    if(speed <= 0){
        print_err("Invalid Speed");
        return -2;
    }
    //make sure not already open
    if(find_dcb(dev) != NULL){
        print_err("port already open");
        return -103;
    }
    //check event flag pointer


    //initialize dcb
    struct dcb* newdcb = (struct dcb*) sys_alloc_mem(sizeof(dcb));
    if (!newdcb) return -104;
    //Placing in array
    dcb_array[dcbArrLoc(dev)] =  newdcb;
    newdcb->dev = dev;
    newdcb->status = 0;
    newdcb->event_flag = 0;
    newdcb->counter = 0;
    newdcb->head = 0;
    newdcb->tail = 0;



    // Calculates baud rate based on desired speed
    int baud_rate_div = 115200 / (long) speed;
    //Making sure baud rate is valid
    if(baud_rate_div>0xFFFF){
        print_err("Baud Rate Speed is invalid!");
        return -102;
    }
    // Installs appropriate ISR in vector table
    idt_install(0x24, serial_isr); //Need the serial_interrupt set up so it can work

    //Set LCR to 0x80 to allow first 2 port registers to access baud rate div register
    outb(dev+3,0x80);
    //Low Order in LSB register
    outb(dev+0, baud_rate_div & 0xFF);
    //High Order in MSB register
    outb(dev+1, (baud_rate_div>>8));
    outb(dev+3,0x03);
    //Enable PIC Register
    //COM1 & COM3 are 4 and COM2 & COM4 are 3
    if(dev == COM1 || dev == COM3){
        cli();
        int mask = inb (0x21);
        mask &= ~(1 << 4); // Enables hardware IRQ 4
        outb (0x21, mask);
        sti();
    }
    else{
        cli();
        int mask = inb (0x21);
        mask &= ~(1<< 3); // Enables hardware IRQ 3
        outb (0x21, mask);
        sti();
    }
    //Enable Interrupts in Modern Control Register
    outb(dev+4,0x08);
    //Enable Interrupts in Interrupt Control Register
    outb(dev+1,0x01);

    return 0;  
}

int serial_close(device dev)
{
    //Check to see if the device is valid
    if(dev!= COM1 && dev!= COM2 && dev!= COM3 && dev!= COM4){
        print_err("Not a valid device!");
        return -1;
    }
    //find the dcb for the device and see if valid
    dcb* d = dcb_array[dcbArrLoc(dev)];
    if(d==NULL || d->status == 0){
        print_err("Serial port not open!");
        return -201;
    }
    //change to closed
    d->status = 1;
    //Disable PIC Register
        if(dev == COM1 || dev == COM3){
        cli();
        int mask = inb(0x21);
        mask |= (1 << 4); // Mask ( disable ) hardware IRQ 4
        outb (0x21, mask);
        sti();
    }
    else{
        cli();
        int mask = inb(0x21);
        mask |= (1 << 3); // Mask ( disable ) hardware IRQ 3
        outb (0x21, mask);
        sti();
    }
    //Disable Interrupts in MSR and IER
    outb(dev+4,0x00);
    outb(dev+1, 0x00);
    return 0;
}

int serial_read(device dev, char* buf, size_t len)
{
    dcb* retrieved_dcb = find_dcb(dev);

    if (find_dcb(dev) == NULL) // not idle dcb
    {
        return -301;
    }
    else if (buf == NULL) // Checks if the buffer is valid
    {
        return -302;
    }
    else if (retrieved_dcb->status != 0) // Checks if the device is busy
    {
        return -304;
    }
    else if (len <= 0) // Checks if the len is valid
    {
        return -303;
    }


    retrieved_dcb->curr_iocb->buffer = buf;
    retrieved_dcb->curr_iocb->buffer_size = len;
    retrieved_dcb->curr_iocb->operation = READ;

    retrieved_dcb->event_flag = 0;

    size_t size = retrieved_dcb->ring_size;

    cli();
    for (size_t i = 0; i < size; i++)
    {
        if (i == len || retrieved_dcb->curr_iocb->buffer[i] == '\n')
        {
            return -1;
        }

        retrieved_dcb->status = 0; 

        retrieved_dcb->curr_iocb->buffer[i] = retrieved_dcb->ring_buf[i];
        retrieved_dcb->ring_buf[i] = 0;
    }
    sti();
    return -1;
}


int serial_write(device dev, char *buf, size_t len)
{
    // Retrieves dcb from an array of them dcbs
    dcb* retrieved_dcb = find_dcb(dev);

    // Sets the new_iocb equal to the params
    iocb* curr_iocb = retrieved_dcb->curr_iocb;
    buf = curr_iocb->buffer;
    len = curr_iocb->buffer_size;

    if (find_dcb(dev) == NULL) // not idle dcb
    {
        return -401;
    }
    else if (buf == NULL) // Checks if the buffer is valid
    {
        return -402;
    }
    else if (retrieved_dcb->status != 0) // Checks if the device is busy
    {
        return -404;
    }
    else if (len <= 0) // Checks if the len is valid
    {
        return -403;
    }


    size_t transferred = retrieved_dcb->counter; // Stores index of transfer locally
    retrieved_dcb->status = 1; // The device is now in use
    retrieved_dcb->event_flag = 0; // Event hasn't finished

    outb(dev, buf[transferred]);

    // Increments the counter to transfer
    transferred = transferred + 1;
    retrieved_dcb->counter = transferred;

    // Set status of retrieved dcb
    retrieved_dcb->status = 3; // Writing
    retrieved_dcb->event_flag = 0; // Incomplete


    int ier_reg = 1;
    int ier = inb(dev + ier_reg);
    ier |= 0x02;

    outb(dev + ier_reg, ier);

    return 0;
    
}
