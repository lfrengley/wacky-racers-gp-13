/** @file   buzzer.h
    @author Lucy Frengley
            Hayden Evans
            Penelope Keeling
            Michael Spurdle
    @date   28 April 2024
    @brief  TODO: Write a description
*/
#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>
// #include "piezo.h"
#include "pwm.h"
#include "pitches.h"
#include "usb_serial.h"
#include "panic.h"
// #include "this_aint_texas.h"




#define BUMP_TIME_MS 5000
#define NUM_NOTES 26
// #define NUM_NOTES 28
#define SONG_MS_PER_NOTE 68
// #define SONG_MS_PER_NOTE 80
#define LEN_SONG NUM_NOTES * SONG_MS_PER_NOTE




void init_buzzer (void);
// Turn the buzzer off, at the initialised frequency. Resets song index to 0
void reset_buzzer (void);

void set_buzzer_high (void);
// Play current buzzer frequency
void play_current_freq (void);

uint32_t get_current_freq (void);

#endif