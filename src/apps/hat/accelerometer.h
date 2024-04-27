/** @file   accelerometer.h
    @author Lucy Frengley
            Hayden Evans
            Penelope Keeling
            Michael Spurdle
    @date   28 April 2024
    @brief  TODO: Write a description
*/
#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <stdbool.h>
#include <stdint.h>

#define NUM_SAMPELS 3

void init_accelerometer(void);

bool check_accelerometer(void);

void set_duty(uint8_t *duty1, uint8_t *duty2, uint8_t *duty3, uint8_t *duty4);

#endif