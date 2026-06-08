#include <string.h>
#include "memory.h"
#include <mpx/serial.h>
#include <mpx/print.h>
#include <mpx/functions.h>
#include <sys_req.h>

#define ALARM_MSG_LEN 64

typedef struct alarm {
    char message[ALARM_MSG_LEN];
    int hour;
    int minute;
    int second;
    struct alarm *next;
} alarm;


// Create alarm struct
alarm* alarm_create(char *msg, int h, int m, int s);

// Add alarm to list. Returns -1 if not alarm is null. Returns 1 if successful.
int alarm_add(char *msg, int h, int m, int s);


// Check alarms against current time and prints its message if it has passed
void alarm_check(void);

// Alarm process, started by commhand command
void running_alarm(void);

// Removes alarm from global alarm struct. Returns -1 if not found. Returns 1 if successful.
int alarm_remove(alarm *a);
