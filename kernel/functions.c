#include <string.h>
#include <mpx/functions.h>
#include <mpx/io.h>
#include <mpx/pcb.h>
#include <memory.h>
#include <stdlib.h>

int kernel_get_day(void){
    outb(0x70,0x07);
    return btoi(inb(0x71));

}

int kernel_get_month(void){
    outb(0x70,0x08);
    return btoi(inb(0x71));
}

int kernel_get_year(void){
    outb(0x70,0x09);
    return btoi(inb(0x71));
}

int kernel_get_hour(void){
    outb(0x70,0x04);
    return btoi(inb(0x71));
}
int kernel_get_minute(void){
    outb(0x70,0x02);
    return btoi(inb(0x71));
}

int kernel_get_second(void){
    outb(0x70,0x00);
    return btoi(inb(0x71));
}

void kernel_set_time_second(int second){
    outb(0x70,0x00);
    outb(0x71,itob(second));
}

void kernel_set_time_minute(int minute){
    outb(0x70,0x02);
    outb(0x71,itob(minute));  
}

void kernel_set_time_hour(int hour){
    outb(0x70,0x04);
    outb(0x71,itob(hour)); 
}

void kernel_set_date_month(int month){
    outb(0x70,0x08);
    outb(0x71,itob(month));
}

void kernel_set_date_day(int day){
    outb(0x70,0x07);
    outb(0x71,itob(day));
}

void kernel_set_date_year(int year){
    outb(0x70,0x09);
    outb(0x71,itob(year));
}
