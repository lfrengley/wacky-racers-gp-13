/** @file   buzzer.c
    @author Lucy Frengley
            Hayden Evans
            Penelope Keeling
            Michael Spurdle
            M. P. Hayes
    @date   25 April 2024
    @brief  TODO: Write a description
*/

#include "piezo.h"
#include "target.h"

piezo_t BUZZER;

static piezo_cfg_t piezo_cfg =
{
    .pio = PA13_PIO
};

void init_buzzer (void) {
    BUZZER = piezo_init(&piezo_cfg);
    if (! BUZZER) {
        panic (LED_ERROR_PIO, 1);
    }
}