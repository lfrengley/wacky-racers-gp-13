/** @file   battery_detect.h
    @author Lucy Frengley
            Hayden Evans
            Penelope Keeling
            Michael Spurdle
    @date   17 May 2024
    @brief  TODO: Write a description
*/

#ifndef BATTERY_DETECT_H
#define BATTERY_DETECT_H

void init_battery_detect (void);

uint16_t calculate_battery_average (void);

void poll_charge_status (void);

#endif