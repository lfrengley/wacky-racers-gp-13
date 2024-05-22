/** @file   led_strip_blink.h
    @author Lucy Frengley
            Hayden Evans
            Penelope Keeling
            Michael Spurdle
            M. P. Hayes
    @date   4 May 2024
    @brief  TODO: Write a description
*/

#ifndef LED_STRIP_BLINK_H
#define LED_STRIP_BLINK_H

#include "../hat/pitches.h"


void init_led_strip (void);
void restart_led_strip (void);
void update_led_strip (void);

void update_racer_led_strip (void);

void set_strip_mode(int16_t duty_left, int16_t duty_right);

void react_to_music (void);

void turn_off_strip (void);
#endif