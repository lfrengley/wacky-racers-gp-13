#include "pitches.h"
#include "durations.h"
#include <stdint.h>
#define NUM_NOTES 26

// #define SONG_BPM 110
// #define NOTE_DIVIDER 8
// #define SONG_NOTE_CHANGE_PER_MIN SONG_BPM * NOTE_DIVIDER // eg 1/8th notes per min
// #define SONG_MS_PER_NOTE 60000 / SONG_NOTE_CHANGE_PER_MIN // eg x ms per 1/8th note duration
#define SONG_MS_PER_NOTE 68

const uint32_t notes[NUM_NOTES] = {
    NOTE_D3, NOTE_A4, NOTE_D4, NOTE_REST, NOTE_D4, NOTE_B3,
    NOTE_D3, NOTE_A4, NOTE_D4, NOTE_A3, NOTE_B3, NOTE_A4, NOTE_D4,
    NOTE_D3, NOTE_A4, NOTE_D4, NOTE_REST, NOTE_D4, NOTE_B3,
    NOTE_D3, NOTE_REST, NOTE_B3, NOTE_D4, NOTE_REST, NOTE_D4, NOTE_B3
};

const uint8_t durations[NUM_NOTES] = {
    2,1,1,1,1,2,
    2,1,1,1,1,1,1,
    2,1,1,1,1,2,
    2,2,1,1,1,3,2,
};