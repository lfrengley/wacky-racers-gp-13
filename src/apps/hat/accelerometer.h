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

#define NUM_SAMPLES 4 //set to 4 as we're polling accelerometer every 2 ms, and radio every 10ms

typedef struct {
    int16_t left;
    int16_t right;
} MotorDuties;

void init_accelerometer(void);

bool check_accelerometer(MotorDuties *duties);

#endif