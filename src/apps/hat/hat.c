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
#include "../libs/battery_detect.h"
#include <stdbool.h>
#include "radio.h"
#include "buzzer.h"


#define LOW_V_ADC 2755
#define ADC_RANGE 4095

/* TASK RATES AND ID'S*/
#define ACCEL_POLL_RATE 10
int accel_task_id;
#define STATUS_LED_BLINK_RATE 1000
int status_led_task_id;
int battery_led_task_id;
#define LED_STRIP_UPDATE_RATE 70
int led_strip_normal_task_id;
#define LED_STRIP_MUSIC_UPDATE_RATE 10
int led_strip_music_task_id;
#define DIP_POLL_RATE 500
int dip_poll_rate_task_id;
#define CHARGE_STATUS_POLL_RATE 30
int charge_status_task_id;
#define BATTERY_POLL_RATE 400
int battery_task_id;

#define LED_STRIP_UPDATE_RATE 70
#define CHECK_BUMP_POLL_RATE SONG_MS_PER_NOTE
int bump_task_id;

MotorDuties duties;
bool listening = true;
bool bump = false;
// bool bump = true;
int32_t bump_start_time_ms = 0;

void toggle_status_led(void) {
    pio_output_toggle (LED_STATUS_PIO);
    // printf("Toggling Status LED\n");
}

void toggle_battery_led(void) {
    pio_output_toggle (LED_ERROR_PIO);
    // printf("Toggling Status LED\n");
}

//TODO: play a song for 5 seconds if bump is true and then set bump to false after
void communicate(void) {
    radio_read_bump(&bump);
    if (bump) {
        rx_to_tx();
        radio_write_duties(0, 0);
    } else if (check_accelerometer(&duties)) {
        rx_to_tx();
        if (radio_write_duties(duties.left, duties.right)) {
        }
    }
}

void check_battery(void) {
    uint16_t battery = calculate_battery_average();
    printf ("Average Battery: %d\n", battery);
    // if (battery < LOW_V_ADC) {
    //     enable_task(battery_led_task_id);
    //     disable_task(led_strip_normal_task_id);
    //     turn_off_strip ();
    //     disable_task (accel_task_id);
    //     disable_task (dip_poll_rate_task_id);

    // } else { //Maybe not reenable
    //     disable_task(battery_led_task_id);
    //     enable_task(led_strip_normal_task_id);
    //     enable_task(accel_task_id);
    //     enable_task(dip_poll_rate_task_id);
    // }
}

void check_bump_status (void) {
    static bool prev_bump = false;
    // printf("Checking bump status\n");
    if (!bump && !prev_bump) {
        return; // return immediately if there's no bump signal, without wasting clock cycles
    }

    if (bump && !prev_bump) { // Rising edge of bump, start song
        bump_start_time_ms = sysclock_millis();
        reset_buzzer(); // doubling up on the reset (from falling edge). Currently doing this for certainty.
        play_current_freq();
        enable_task(led_strip_music_task_id);
        disable_task(led_strip_normal_task_id);
    } else if (bump && prev_bump) { // Continue playing song
        play_current_freq();
    } else if (!bump && prev_bump) { // Falling edge of bump, end song
        reset_buzzer();
        disable_task(led_strip_music_task_id);
        enable_task(led_strip_normal_task_id);
        restart_led_strip();

    }
    prev_bump = bump;

    if (sysclock_millis() - bump_start_time_ms > BUMP_TIME_MS) {
        bump = false;
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

    // Initialise battery
    init_battery_detect();

    // Initialise Accelerometer
    init_accelerometer();

    // Initialise Radio
    init_radio_dips(); //Must do before init radio
    init_radio();

    // Initialise Led Strip
    init_led_strip();

    // Initialise Piezo
    init_buzzer();
    // reset_buzzer();

    // Initialise Piezo
    init_buzzer();
    // reset_buzzer();

    // Initialise TasksG
    status_led_task_id = add_task(&toggle_status_led, STATUS_LED_BLINK_RATE);
    battery_led_task_id = add_task(&toggle_battery_led, STATUS_LED_BLINK_RATE);
    disable_task(battery_led_task_id);

    accel_task_id = add_task(&communicate, ACCEL_POLL_RATE);
    // disable_task(accel_task_id);
    led_strip_normal_task_id = add_task(&update_led_strip, LED_STRIP_UPDATE_RATE);
    led_strip_music_task_id = add_task(&react_to_music, LED_STRIP_MUSIC_UPDATE_RATE);
    disable_task(led_strip_music_task_id);
    dip_poll_rate_task_id = add_task(&poll_radio_dips, DIP_POLL_RATE);

    // charge_status_task_id = add_task(&poll_charge_status, CHARGE_STATUS_POLL_RATE);
    // battery_task_id = add_task(&check_battery, BATTERY_POLL_RATE);
    // disable
    bump_task_id = add_task(&check_bump_status, CHECK_BUMP_POLL_RATE);
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