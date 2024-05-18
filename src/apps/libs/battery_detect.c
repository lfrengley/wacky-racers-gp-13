/** @file   battery_detect.c
    @author Lucy Frengley
            Hayden Evans
            Penelope Keeling
            Michael Spurdle
    @date   17 May 2024
    @brief  TODO: Write a description
*/

#include "target.h"
#include "pio.h"

void init_battery_detect (void) {
    pio_config_set (BATTERY_DETECT_PIO, PIO_INPUT);
}

void poll_charge_status (void) {
    uint8_t charge = pio_input_get (BATTERY_DETECT_PIO);
    printf ("Charge status = %d", charge);
}