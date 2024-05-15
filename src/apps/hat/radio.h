/** @file   radio.h
    @author Lucy Frengley
            Hayden Evans
            Penelope Keeling
            Michael Spurdle
    @date   8 May 2024
    @brief  TODO: Write a description
*/
#ifndef RADIO_H
#define RADIO_H

#include <stdint.h>
#include <stdbool.h>
#include "panic.h"

void init_radio (void);

void init_radio (void);

bool radio_write_duties (int16_t left, int16_t right);

void radio_read_bump (bool *bump);

void rx_to_tx (void);

#endif