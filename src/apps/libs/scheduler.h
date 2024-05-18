#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "sam4s/sysclock/sysclock.h"
#include <stdlib.h>
#include "panic.h"

void disable_task(int task_id);

void enable_task(int task_id);

int add_task(void (*task_function)(void), unsigned long period);

void run_scheduler(void);
#endif