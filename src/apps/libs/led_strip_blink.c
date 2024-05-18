/** @file   led_strip_blink.c
    @author Lucy Frengley
            Hayden Evans
            Penelope Keeling
            Michael Spurdle
    @date   16 May 2024
    @brief  TODO: Write a description
*/

#include "ledbuffer.h"
#include "target.h"

#define NUM_LEDS 21

bool moving = false;
bool prev_moving = true;
int8_t direction = 1;
uint8_t red_level = 255;
ledbuffer_t *leds;
bool start = true;

void init_led_strip (void) {
    leds = ledbuffer_init (LEDTAPE_PIO, NUM_LEDS);
}

void turn_off_strip () {
    ledbuffer_clear(leds);
    ledbuffer_write (leds);
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