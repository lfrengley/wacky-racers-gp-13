/** @file   hat.c
    @author Lucy Frengley
            Hayden Evans
            Penelope Keeling
            Michael Spurdle
            M. P. Hayes
    @date   25 April 2024
    @brief  TODO: Write a description
*/

#include "target.h"
#include "pio.h"
#include "pacer.h"
#include "usb_serial.h"
#include "accelerometer.h"
#include <stdbool.h>

#define PACER_RATE 20
#define ACCEL_POLL_RATE 1

/* Initialise */
void init(void) {
    // Redirect stdio to USB serial
    usb_serial_stdio_init ();

    // Initialise PIO pins
    pio_config_set (LED_ERROR_PIO, PIO_OUTPUT_LOW);
    pio_output_set (LED_ERROR_PIO, ! LED_ACTIVE);
    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_LOW);
    pio_output_set (LED_STATUS_PIO, ! LED_ACTIVE);

    // Initialise Accelerometer
    init_accelerometer ();

    pacer_init (PACER_RATE);
}

/* Begin */
int
main (void)
{
    int ticks = 0;
    uint8_t duty1;
    uint8_t duty2;
    uint8_t duty3;
    uint8_t duty4;
    //Initialise
    init();

    // TODO: write hat program here...
    while (1) {
        pacer_wait ();

        ticks++;
        if (ticks < PACER_RATE / ACCEL_POLL_RATE)
            continue;
        ticks = 0;

        pio_output_toggle (LED_STATUS_PIO);

        if (check_accelerometer()) {
            set_duty(&duty1, &duty2, &duty3, &duty4);
            printf ("duty1: %3d, duty2: %3d, duty3: %3d, duty4: %3d \n\n", duty1, duty2, duty3, duty4);
        }

        
        // TODO: send to radio!
    }
}