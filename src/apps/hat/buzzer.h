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
#include "piezo.h"
#include "pwm.h"
#include "pitches.h"

#define BUMP_TIME_MS 5000

#define SONG_BPM 132
#define EIGHTH_NOTE 8
#define EIGHTH_NOTE_DURATION_MS SONG_BPM / EIGHTH_NOTE

// #define NUM_NOTES EIGHTH_NOTE_DURATION_MS * BUMP_TIME_MS
// #define LEN_SONG EIGHTH_NOTE_DURATION_MS * NUM_NOTES
#define NUM_NOTES 310 //manually counted (with python) test array
#define LEN_SONG NUM_NOTES * EIGHTH_NOTE_DURATION_MS

void init_buzzer (void);

// Turn the buzzer off, at the initialised frequency. Resets song index to 0
void reset_buzzer (void);


// Sets the buzzer to the next frequency in the song array
void play_next_freq (void);


#endif