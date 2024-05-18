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
#include "../libs/led_strip_blink.h"
#include <stdbool.h>
#include "radio.h"

/* TASK RATES AND ID'S*/
#define ACCEL_POLL_RATE 10
int accel_task_id;
#define STATUS_LED_BLINK_RATE 1000
int status_led_task_id;
#define LED_STRIP_UPDATE_RATE 70
int led_strip_task_id;
#define DIP_POLL_RATE 500
int dip_poll_rate_task_id;

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
    init_radio_dips(); //Must do before init radio
    init_radio();

    // Initialise Led Strip
    init_led_strip();

    // Initialise Tasks
    status_led_task_id = add_task(&toggle_status_led, STATUS_LED_BLINK_RATE);
    accel_task_id = add_task(&communicate, ACCEL_POLL_RATE);
    led_strip_task_id = add_task(&update_led_strip, LED_STRIP_UPDATE_RATE);
    dip_poll_rate_task_id = add_task(&poll_radio_dips, DIP_POLL_RATE);
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