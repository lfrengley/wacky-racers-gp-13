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

#define BUMP_TIME_MS 5000
#define SONG_BPM 132
#define NOTE_DIVIDER 8
#define SONG_NOTE_CHANGE_PER_MIN SONG_BPM * NOTE_DIVIDER // eg 1/8th notes per min
// #define SONG_MS_PER_NOTE 60000 / SONG_NOTE_CHANGE_PER_MIN // eg x ms per 1/8th note duration
#define SONG_MS_PER_NOTE 78 //57 for scoooby dooo

// #define NUM_NOTES EIGHTH_NOTE_DURATION_MS * BUMP_TIME_MS
// #define LEN_SONG EIGHTH_NOTE_DURATION_MS * NUM_NOTES
#define NUM_NOTES 33 //manually counted (with python) test array
#define LEN_SONG NUM_NOTES * SONG_MS_PER_NOTE

#define DURATION_EIGHTH 1
#define DURATION_QUARTER 2
#define DURATION_HALF 4
#define DURATION_FULL 8

void init_buzzer (void);
// Turn the buzzer off, at the initialised frequency. Resets song index to 0
void reset_buzzer (void);

void set_buzzer_high (void);
// Play current buzzer frequency
void play_current_freq (void);


#endif