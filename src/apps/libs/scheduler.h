#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "sam4s/sysclock/sysclock.h"
#include <stdlib.h>
#include "panic.h"

/* TASK RATES AND ID'S*/
#define ACCEL_POLL_RATE 10
int accel_task_id;
#define STATUS_LED_BLINK_RATE 1000
int status_led_task_id;
#define LED_STRIP_UPDATE_RATE 70
int led_strip_task_id;
#define DIP_POLL_RATE 500
int dip_poll_rate_task_id;

void disable_task(int task_id);
int add_task(void (*task_function)(void), unsigned long period);
void run_scheduler(void);
// void sched_init(void);

#endif