/** @file   led_strip_blink.c
    @author Lucy Frengley
            Hayden Evans
            Penelope Keeling
            Michael Spurdle
    @date   16 May 2024
    @brief  TODO: Write a description
*/

#include "led_strip_blink.h"
#include "ledbuffer.h"
#include "target.h"
#include "buzzer.h"

#define NUM_LEDS 21

bool moving = false;
bool prev_moving = true;
int8_t direction = 1;
uint8_t red_level = 255;
ledbuffer_t *leds;
bool start = true;

#define NOTES_PER_COLOUR 16
uint32_t A_notes[NOTES_PER_COLOUR] = {NOTE_A1, NOTE_A2, NOTE_A3, NOTE_A4, NOTE_A5, NOTE_A6, NOTE_A7, 0, NOTE_AS1, NOTE_AS2, NOTE_AS3, NOTE_AS4, NOTE_AS5, NOTE_AS6, NOTE_AS7, 0};
uint32_t B_notes[NOTES_PER_COLOUR] = {NOTE_B1, NOTE_B2, NOTE_B3, NOTE_B4, NOTE_B5, NOTE_B6, NOTE_B7,0,0,0,0,0,0,0,0}; 
uint32_t C_notes[NOTES_PER_COLOUR] = {NOTE_C1, NOTE_C2, NOTE_C3, NOTE_C4, NOTE_C5, NOTE_C6, NOTE_C7, NOTE_C8, NOTE_CS1, NOTE_CS2, NOTE_CS3, NOTE_CS4, NOTE_CS5, NOTE_CS6, NOTE_CS7, NOTE_CS8};
uint32_t D_notes[NOTES_PER_COLOUR] = {NOTE_D1, NOTE_D2, NOTE_D3, NOTE_D4, NOTE_D5, NOTE_D6, NOTE_D7, NOTE_D8, NOTE_DS1, NOTE_DS2, NOTE_DS3, NOTE_DS4, NOTE_DS5, NOTE_DS6, NOTE_DS7, NOTE_DS8};
uint32_t E_notes[NOTES_PER_COLOUR] = {NOTE_E1, NOTE_E2, NOTE_E3, NOTE_E4, NOTE_E5, NOTE_E6, NOTE_E7,0,0,0,0,0,0,0,0};
uint32_t F_notes[NOTES_PER_COLOUR] = {NOTE_F1, NOTE_F2, NOTE_F3, NOTE_F4, NOTE_F5, NOTE_F6, NOTE_F7, 0, NOTE_FS1, NOTE_FS2, NOTE_FS3, NOTE_FS4, NOTE_FS5, NOTE_FS6, NOTE_FS7, 0};
uint32_t G_notes[NOTES_PER_COLOUR] = {NOTE_G1, NOTE_G2, NOTE_G3, NOTE_G4, NOTE_G5, NOTE_G6, NOTE_G7, 0, NOTE_GS1, NOTE_GS2, NOTE_GS3, NOTE_GS4, NOTE_GS5, NOTE_GS6, NOTE_GS7, 0};


void init_led_strip (void) {
    leds = ledbuffer_init (LEDTAPE_PIO, NUM_LEDS);
}

void turn_off_strip () {
    ledbuffer_clear(leds);
    ledbuffer_write (leds);
}

void restart_led_strip (void) {
    start = true;
}

void update_led_strip (void) {
    if (start) {
        ledbuffer_clear(leds);
        ledbuffer_set(leds, 0, 255, 0, 0);   //R
        ledbuffer_set(leds, 3, 255, 80, 0);  //O
        ledbuffer_set(leds, 6, 240, 255, 0); //Y
        ledbuffer_set(leds, 9, 0, 255, 0);   //G
        ledbuffer_set(leds, 12, 0, 0, 255);  //B
        ledbuffer_set(leds, 15, 120, 0, 255);//I
        ledbuffer_set(leds, 18, 255, 0, 100);//V
        start = false;
    }
    ledbuffer_write (leds);
    ledbuffer_advance (leds, 1);
}


void update_racer_led_strip (void) {
    if (!moving) {
        if (prev_moving) {
            ledbuffer_clear(leds);
            prev_moving = moving;
        }
        for (size_t i = 0; i < NUM_LEDS; i++) {
            ledbuffer_set(leds, i, red_level, 0, 0);   //R
        }
        ledbuffer_write (leds);
        red_level = red_level - 5;
    }
    
    if (moving) {
        if (!prev_moving) {
            ledbuffer_clear(leds);
            ledbuffer_set(leds, 0, 255, 0, 0);   //R
            ledbuffer_set(leds, 3, 255, 80, 0);  //O
            ledbuffer_set(leds, 6, 240, 255, 0); //Y
            ledbuffer_set(leds, 9, 0, 255, 0);   //G
            ledbuffer_set(leds, 12, 0, 0, 255);  //B
            ledbuffer_set(leds, 15, 120, 0, 255);//I
            ledbuffer_set(leds, 18, 255, 0, 100);//V
            prev_moving = moving;
        }
        ledbuffer_write (leds);
        ledbuffer_advance (leds, direction);
    }
}

void set_strip_mode(int16_t duty_left, int16_t duty_right) {
    if (!duty_left && !duty_right) {
        moving = false;
        direction = 0;
    } else if ((duty_left > 0) && (duty_right > 0)) {
        moving = true;
        direction = 1;
    } else if ((duty_left < 0) && (duty_right < 0)) {
        moving = true;
        direction = -1;
    } else {
        moving = true;
        direction = 0;
    }
}


bool freq_in_colour (uint32_t notes[], uint32_t freq) {
    for (int i=0; i<NOTES_PER_COLOUR; i++) {
        if (freq == notes[i]) {
            return true;
        }
    }
    return false;
}
void map_freq_to_colour (uint32_t freq, uint8_t* r, uint8_t* g, uint8_t* b) {
    if (freq_in_colour(A_notes, freq)) { //light green
        *r = 144;
        *g = 238;
        *b = 144;
    } else if (freq_in_colour(B_notes, freq)) { //sky blue?
        *r = 75;
        *g = 170;
        *b = 255;
    } else if (freq_in_colour(C_notes, freq)) { // red
        *r = 255;
        *g = 0;
        *b = 0; 
    } else if (freq_in_colour(D_notes, freq)) { // yellow
        *r = 255;
        *g = 255;
        *b = 0; 
    } else if  (freq_in_colour(E_notes, freq)) { //light blue
        *r = 68;
        *g = 85;
        *b = 90;
    } else if (freq_in_colour(F_notes, freq)) { // violet
        *r = 218;
        *g = 188;
        *b = 255;
    } else if (freq_in_colour(G_notes, freq)) { // Orange
        *r = 255;
        *g = 165;
        *b = 0;
    } else {
        *r = 255;
        *g = 255;
        *b = 255; 
    }
}

void react_to_music (void) {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    uint32_t freq = get_current_freq();
    map_freq_to_colour(freq, &r,&g, &b);

    for (int i=0; i<NUM_LEDS; i++) {
        ledbuffer_set(leds, i, r,g,b);
    }
    ledbuffer_write(leds);
}