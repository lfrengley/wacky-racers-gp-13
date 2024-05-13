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
#include "buzzer.h"


#define PACER_RATE 20
#define ACCEL_POLL_RATE 10
#define STATUS_LED_BLINK_RATE 1000
#define CHECK_BUMP_POLL_RATE SONG_MS_PER_NOTE

MotorDuties duties;
bool listening = true;
bool bump = false;
int32_t bump_start_time_ms = 0;

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

void check_bump_status (void) {
    static bool prev_bump = false;
    if (!bump && !prev_bump) {
        return; // return immediately if there's no bump signal, without wasting clock cycles
    }

    // if (sysclock_millis() - bump_start_time_ms > BUMP_TIME_MS) {
    //     bump = false;
    // }

    if (bump && !prev_bump) { // Rising edge of bump, start song
        bump_start_time_ms = sysclock_millis();
        reset_buzzer(); // doubling up on the reset (from falling edge). Currently doing this for certainty.
        play_next_freq();
    } else if (bump && prev_bump) { // Continue playing song
        play_next_freq();
    } else if (!bump && prev_bump) { // Falling edge of bump, end song
        reset_buzzer();
    }
    prev_bump = bump;
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

    // Initialise Piezo
    init_buzzer();

    // Initialise Song Array
    if (!init_song()) {
        printf("Failed to init song array");
    }

    // Initialise Tasks
    add_task(&toggle_status_led, STATUS_LED_BLINK_RATE);
    // add_task(&communicate, ACCEL_POLL_RATE);
    add_task(&check_bump_status, CHECK_BUMP_POLL_RATE);
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