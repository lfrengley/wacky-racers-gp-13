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

#define BUMP_TIME_MS 5000

#define SONG_BPM 132
#define NOTE_DIVIDER 8
#define SONG_NOTE_CHANGE_PER_MIN SONG_BPM * NOTE_DIVIDER // eg 1/8th notes per min
#define SONG_MS_PER_NOTE 60000 / SONG_NOTE_CHANGE_PER_MIN // eg x ms per 1/8th note duration

// #define NUM_NOTES EIGHTH_NOTE_DURATION_MS * BUMP_TIME_MS
// #define LEN_SONG EIGHTH_NOTE_DURATION_MS * NUM_NOTES
#define NUM_NOTES 310 //manually counted (with python) test array
#define LEN_SONG NUM_NOTES * SONG_MS_PER_NOTE

void init_buzzer (void);
// Turn the buzzer off, at the initialised frequency. Resets song index to 0
void reset_buzzer (void);


// Play current buzzer frequency
void play_current_freq (void);


#endif