#ifndef SLEEP_INTERRUPTZ_H
#define SLEEP_INTERRUPTZ_H

#include "target.h"
#include "mcu_sleep.h"
#include "config.h"
#include "mcu.h"
#include "cpu.h"
#include "irq.h"

//Button includes:
#include "pio.h"
#include "pacer.h"
#include "button.h"

void sleeper_stuff (void);

void init_sleeper(void);

#endif