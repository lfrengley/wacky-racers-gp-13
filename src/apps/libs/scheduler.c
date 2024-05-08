#include "scheduler.h"


#define MAX_TASKS 10

// Task structure
typedef struct {
    void (*task_function)(void);
    unsigned long period;  // Period in microseconds
    unsigned long last_executed; // Last time the task was executed
} Task;

Task tasks[MAX_TASKS];
int num_tasks = 0;

void add_task(void (*task_function)(void), unsigned long period) {
    if (num_tasks >= MAX_TASKS) {
        printf("Cannot add more tasks, reached abitrary maximum limit\n");
        return;
    }
    tasks[num_tasks].task_function = task_function;
    tasks[num_tasks].period = period;
    tasks[num_tasks].last_executed = 0;
    num_tasks++;
}

void run_scheduler(void) {
    uint32_t current_time;
    int i;

    while (1) {
        current_time = sysclock_micros();
        for (i = 0; i < num_tasks; i++) {
            if (current_time - tasks[i].last_executed >= tasks[i].period) {
                tasks[i].task_function();
                tasks[i].last_executed = current_time;
            }
        }
        uint32_t scheduler_delay = 1; //micros
        sysclock_micros_delay(scheduler_delay);
    }
}

