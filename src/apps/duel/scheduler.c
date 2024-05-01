#include "scheduler.h"


void sched_init(void) {
    systick_clocks_t period = 300000;
    if (systick_init(period)) {
        print("Systick Init Success");
    }
}