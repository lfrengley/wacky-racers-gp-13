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

ledbuffer_t *leds;
bool start = true;

void init_led_strip (void) {
    leds = ledbuffer_init (LEDTAPE_PIO, NUM_LEDS);
}

void update_led_strip (void) {
    if (start) {
        // wait for a revolution
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