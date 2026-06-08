#include <mpx/alarm.h>
#include <mpx/pcb.h>
#include <../include/stdlib.h> // some compilers assumed functions declared in standard stdlib.h if not delcared explicitly
#include <mpx/color.h>

#define ALARM_MSG_LEN 64

static alarm *alarmHead = NULL;



alarm* alarm_create(char *msg, int h, int m, int s)
{
    alarm *newalarm = (alarm*) sys_alloc_mem(sizeof(alarm));

    if (newalarm == NULL)
        return NULL;

    //insert message
    int i;
    for (i = 0; i < ALARM_MSG_LEN - 1 && msg[i] != '\0'; i++)
    {
        newalarm->message[i] = msg[i];
    }
    newalarm->message[i] = '\0'; // null-terminate

    newalarm->hour = h;
    newalarm->minute = m;
    newalarm->second = s;
    newalarm->next = NULL;

    return newalarm;
}

int alarm_add(char *msg, int h, int m, int s)
{
    alarm *newalarm = alarm_create(msg, h, m, s);
    if (newalarm == NULL)
        return -1;

    if (alarmHead == NULL)
    {
        alarmHead = newalarm;
        return 0;
    }

    alarm *current = alarmHead;
    while (current->next != NULL)
    {
        current = current->next;
    }

    current->next = newalarm;
    return 0;
}

int alarm_remove(alarm *t)
{
    if (alarmHead == NULL || t == NULL)
        return -1; 

    alarm *current = alarmHead;
    alarm *prev = NULL;

    while (current != NULL)
    {
        if (current == t)
        {
            // Found the alarm to remove
            if (prev == NULL)
            {
                alarmHead = current->next;
            }
            else
            {
                prev->next = current->next;
            }

            sys_free_mem(current); 
            return 0; 
        }

        prev = current;
        current = current->next;
    }
    return -1;
}

void running_alarm(void)
{
    while(alarmHead != NULL)
    {
        alarm_check();
        sys_req(IDLE);
    }
    sys_req(EXIT);
}

void alarm_check(void)
{
    int s = kernel_get_second();
    int m = kernel_get_minute();
    int h = kernel_get_hour();

    alarm *current_alarm = alarmHead;
    while (current_alarm != NULL)
    {
        if (h > current_alarm->hour || 
            (h == current_alarm->hour && (m > current_alarm->minute || 
                (m == current_alarm->minute && s >= current_alarm->second))))    
        {
            print_col("Alarm Is Ringing: ", MAGENTA);
            print_col(current_alarm->message, MAGENTA);
            print("\n");
            alarm_remove(current_alarm);
        }

        current_alarm = current_alarm->next;
    }
}
