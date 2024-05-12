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
#include "../libs/scheduler.h"
#include <stdbool.h>
#include "radio.h"

#define PACER_RATE 20
#define ACCEL_POLL_RATE 10
#define STATUS_LED_BLINK_RATE 1000

MotorDuties duties;
bool listening = true;
bool bump = false;

void toggle_status_led(void) {
    pio_output_toggle (LED_STATUS_PIO);
    // printf("Toggling Status LED\n");
}

//TODO: play a song for 5 seconds if bump is true and then set bump to false after
void communicate(void) {
    radio_read_bump(&bump);
    if (bump) {
        rx_to_tx();
        radio_write_duties(0, 0);
        //play music for 5 seconds and then set bump to false 
    } else if (check_accelerometer(&duties)) {
        rx_to_tx();
        if (radio_write_duties(duties.left, duties.right)) {
        }
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

    // Initialise Tasks
    add_task(&toggle_status_led, STATUS_LED_BLINK_RATE);
    add_task(&communicate, ACCEL_POLL_RATE);

}

/* Begin */
int
main (void)
{
    int ticks = 0;
   
    //Initialise
    init();
    run_scheduler();
}