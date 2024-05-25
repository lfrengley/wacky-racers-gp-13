#include "scheduler.h"


#define MAX_TASKS 20

// Task structure
typedef struct {
    void (*task_function)(void);
    unsigned long period;  // Period in milliseconds
    unsigned long last_executed; // Last time the task was executed
    bool disabled;
} Task;

Task tasks[MAX_TASKS];
int num_tasks = 0;
int next_available_task_id = 0;

int add_task(void (*task_function)(void), unsigned long period) {
    if (num_tasks >= MAX_TASKS) {
        printf("Cannot add more tasks, reached maximum limit\n");
        return 0;
    }

    tasks[num_tasks].task_function = task_function;
    tasks[num_tasks].period = period;
    tasks[num_tasks].last_executed = 0;
    tasks[num_tasks].disabled = false;
    int index = num_tasks;
    num_tasks++;
    return index;
}

void disable_task(int task_id) {
    tasks[task_id].disabled = true;
}

void enable_task(int task_id) {
    tasks[task_id].disabled = false;
}

void run_scheduler(void) {
    uint32_t current_time;
    int i;

    while (1) {
        current_time = sysclock_millis();
        for (i = 0; i < num_tasks; i++) {
            if (!tasks[i].disabled && (current_time - tasks[i].last_executed >= tasks[i].period)) {
                tasks[i].task_function();
                tasks[i].last_executed = current_time;
            }
        }
        uint32_t scheduler_delay = 1; //ms
        sysclock_millis_delay(scheduler_delay);
    }
}

