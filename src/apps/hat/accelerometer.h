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

typedef struct {
    // int16_t A1;
    // int16_t A2;
    // int16_t A3;
    // int16_t A4;
    int16_t left;
    int16_t right;
} MotorDuties;

void init_accelerometer(void);

bool check_accelerometer(MotorDuties *duties);

#endif