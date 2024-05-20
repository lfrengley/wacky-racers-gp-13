/** @file   buzzer.c
    @author Lucy Frengley
            Hayden Evans
            Penelope Keeling
            Michael Spurdle
            M. P. Hayes
    @date   25 April 2024
    @brief  TODO: Write a description
*/


#include "buzzer.h"
#include "target.h"

#define NOTE_REST 0

const uint32_t notes[NUM_NOTES] = {
NOTE_E4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_G4,
                   NOTE_E4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_REST, NOTE_E4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_G4,
                   NOTE_E4, NOTE_E4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_G4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4};

const uint8_t durations[NUM_NOTES] = {
DURATION_QUARTER, DURATION_QUARTER, DURATION_QUARTER, DURATION_QUARTER, DURATION_QUARTER, DURATION_QUARTER,
                      DURATION_QUARTER, DURATION_QUARTER, DURATION_QUARTER, DURATION_QUARTER, DURATION_QUARTER, DURATION_HALF,
                      DURATION_QUARTER, DURATION_QUARTER, DURATION_QUARTER, DURATION_QUARTER, DURATION_QUARTER, DURATION_EIGHTH,
                      DURATION_QUARTER, DURATION_QUARTER, DURATION_QUARTER, DURATION_QUARTER, DURATION_QUARTER, DURATION_QUARTER,
                      DURATION_QUARTER, DURATION_QUARTER, DURATION_QUARTER, DURATION_QUARTER, DURATION_QUARTER, DURATION_HALF
                      };


static pwm_t PWM_buzzer;
#define INIT_BUZZ_PWM_FREQ 1000 //hz
static pwm_cfg_t pwm_buzz_config =
{
    .pio = BUZZ_PIO,
    .period = PWM_PERIOD_DIVISOR (INIT_BUZZ_PWM_FREQ),
    .duty = PWM_DUTY_DIVISOR (INIT_BUZZ_PWM_FREQ, 0),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_LOW,
    .stop_state = PIO_OUTPUT_LOW
};

void init_buzzer (void) {
    PWM_buzzer = pwm_init (&pwm_buzz_config);
    if (! PWM_buzzer) {
        panic (LED_ERROR_PIO, 1);
    }
    pwm_channels_start (pwm_channel_mask (PWM_buzzer));
}

static int32_t note_index = 0;

void set_buzzer_high(void) {
    pwm_duty_set(PWM_buzzer, PWM_DUTY_DIVISOR(INIT_BUZZ_PWM_FREQ, 50));
}
void play_current_freq (void) {
    // Check how far through the note we are
    static int8_t eighth_beat = 1;
    if (eighth_beat > durations[note_index]) { //if we have played all of the note, move on to the next
        eighth_beat = 1;
        note_index++;
        // printf("Playing note %ld for %d/8 beats\n", notes[note_index], durations[note_index]);
    } else { // increase the note count
        eighth_beat++;
    }

    // Restart song if needed
    if (note_index >= NUM_NOTES) {
        note_index = 0;
        eighth_beat = 1;
    }

    //play the current note
    pwm_frequency_t freq = notes[note_index];
    if (freq == NOTE_REST) {
        uint32_t prev_note = note_index;
        if (note_index == 0) {
            prev_note = NUM_NOTES;
        }
        pwm_duty_set(PWM_buzzer, PWM_DUTY_DIVISOR(prev_note, 0)); // stop the buzzer
    } else {
        pwm_frequency_set(PWM_buzzer, freq);
        pwm_duty_set(PWM_buzzer, PWM_DUTY_DIVISOR(freq, 50));
    }
}

void reset_buzzer (void) {
    pwm_duty_set(PWM_buzzer, PWM_DUTY_DIVISOR(INIT_BUZZ_PWM_FREQ, 0));
    note_index = 0;
}

uint32_t get_current_freq (void) {
    return notes[note_index];
}