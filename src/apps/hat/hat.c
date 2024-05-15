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

#include "sleep_interruptz.h"
#include "mcu_sleep.h"
#include "config.h"
#include "mcu.h"
#include "cpu.h"
#include "irq.h"
#include "button.h"
button_t sleep_button;

#include "sleep_interruptz.h"

#define PACER_RATE 20
#define ACCEL_POLL_RATE 10
#define STATUS_LED_BLINK_RATE 1000
#define LED_STRIP_UPDATE_RATE 70
#define SLEEP_RATE 50


MotorDuties duties;
bool listening = true;
bool bump = false;

void toggle_status_led(void) {
    pio_output_toggle (LED_STATUS_PIO);
    printf("Toggling Status LED\n");
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

    // Initialise Led Strip
    init_led_strip();

    // Init button 
    init_sleeper();

    // Initialise Tasks
    add_task(&toggle_status_led, STATUS_LED_BLINK_RATE);
    // add_task(&communicate, ACCEL_POLL_RATE);
    // add_task(&update_led_strip, LED_STRIP_UPDATE_RATE);
    add_task(&sleeper_stuff, SLEEP_RATE);
}




/* Defines button configuration.  */
static const button_cfg_t sleep_button_cfg =
{
    .pio = SLEEP_BUTTON_PIO
};


/* Defines wakeup configuration */
static mcu_sleep_wakeup_cfg_t wkup_config =
{
    .pio = PA0_PIO, //pin for wakeup
    .active_high = false // Wakeup on high signal = false
};


/* Defines level of MCU power saving */
static mcu_sleep_cfg_t sleep_config =
{
    .mode = MCU_SLEEP_MODE_WAIT //this can be changed to how deep asleep it goes?
};


/* checks the wakeup pin is set, and then puts MCU to sleep */
void enter_sleep_mode(void)
{
    if (!mcu_sleep_wakeup_set(&wkup_config))
    {
        printf("\nuhoh no wakeup set\n");
        return;
    }
    mcu_sleep(&sleep_config);
}

/* Exit from backup mode occurs if there is an event on the
    WKUPEN0-15 pins, supply monitor (SM), RTC alarm, or RTT
    alarm.  The supply monitor monitors the voltage on the
    VDDIO pin if it is enabled.  The MCU is reset.  */


/* Initialises the button */
void init_sleeper(void) {
    sleep_button = button_init (&sleep_button_cfg);
}


/* Main check button function that is periodically called by scheduler */
void sleeper_stuff (void)
{
    button_poll (sleep_button);
    
    if (button_pushed_p (sleep_button)) {
        printf("\nenterning sleep mode func\n");
        enter_sleep_mode();

    }    add_task(&sleeper_stuff, SLEEP_RATE);
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