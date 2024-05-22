/** @file   motors.h
    @author Lucy Frengley
            Hayden Evans
            Penelope Keeling
            Michael Spurdle
            M. P. Hayes
    @date   4 May 2024
    @brief  TODO: Write a description
*/

#ifndef MOTORS_H
#define MOTORS_H

#include <stdint.h>
#include "panic.h"
void init_motors (void);

void set_motor_duties (int16_t left, int16_t right);

#endif