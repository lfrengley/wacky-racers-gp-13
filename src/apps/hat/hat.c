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
#include "scheduler.h"
#include <stdbool.h>
#include "radio.h"

#define PACER_RATE 20
#define ACCEL_POLL_RATE 10
#define STATUS_LED_BLINK_RATE 100


void toggle_status_led(void) {
    pio_output_toggle (LED_STATUS_PIO);
    // printf("Toggling Status LED\n");
}
MotorDuties duties;
void poll_accel(void) {
    if (check_accelerometer(&duties)) {
        printf ("Left Duty: %3d%%, \tRight Duty: %3d%%\n\n", duties.left, duties.right);
        radio_write_duties(duties.left, duties.right);
    }   
}

/* Initialise */
void init(void) {
    // Redirect stdio to USB serial
    usb_serial_stdio_init ();

    // Initialise PIO pins
    pio_config_set (LED_ERROR_PIO, PIO_OUTPUT_LOW);
    pio_output_set (LED_ERROR_PIO, ! LED_ACTIVE);
    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_LOW);
    pio_output_set (LED_STATUS_PIO, ! LED_ACTIVE);

    // Initialise sysclock
    sysclock_init();

    // Initialise Accelerometer
    init_accelerometer();

    // Initialise Radio
    init_radio();

    // init_sleep_butt();

    // Initialise Tasks
    add_task(&toggle_status_led, STATUS_LED_BLINK_RATE);
    add_task(&poll_accel, 250);

    // pacer_init (PACER_RATE);

}

/* Begin */
int
main (void)
{
    int ticks = 0;
   
    //Initialise
    init();
    run_scheduler();
    // TODO: write hat program here...
    // while (1) {
    //     pacer_wait ();

    //     ticks++;
    //     if (ticks < PACER_RATE / ACCEL_POLL_RATE)
    //         continue;
    //     ticks = 0;

    //     pio_output_toggle (LED_STATUS_PIO);

    //     if (check_accelerometer(&duties)) {
    //         printf ("Pseudo Duties-> Left: %3d%%, \tRight: %3d%%\n", duties.left, duties.right);
    //     }
    //     // TODO: send to radio!
    // }
}