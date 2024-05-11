/** @file   racer.c
    @author Lucy Frengley
            Hayden Evans
            Penelope Keeling
            Michael Spurdle
            M. P. Hayes
    @date   4 May 2024
    @brief  TODO: Write a description
*/

#include "target.h"
#include "pio.h"
#include "usb_serial.h"
#include "motors.h"
#include "../libs/scheduler.h"
#include "../libs/radio.h"

#define PACER_RATE 20
#define SERIAL_POLL_RATE 1
#define STATUS_LED_BLINK_RATE 1000

bool listening = true;
bool bump = false;
int16_t duty_left, duty_right;
uint8_t current_bump, previous_bump;


void toggle_status_led(void) {
    pio_output_toggle (LED_STATUS_PIO);
    // printf("Toggling Status LED\n");
}

//TODO: configure actual bump sensor
void check_bump() {
    previous_bump = current_bump;
    current_bump = pio_input_get(RADIO_DIP1_PIO);
    if (previous_bump == 0 && current_bump == 1) {
        bump = true;
    } else {
        bump = false;
    }
}

void communicate(void) {
    check_bump();
    if (listening) {
        if (radio_read_duties(&duty_left, &duty_right)) {
            set_motor_duties(duty_left, duty_right);
        }
        if (bump){
            listening = false;
        }
    }

    if (!listening) {
        rx_to_tx(); // this is based on what is written in rf_tester 
        if (radio_write_bump(true)) {
            listening = true;
        } else {
            radio_read_duties(&duty_left, &duty_right);
            set_motor_duties(duty_left, duty_right);
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
    pio_config_set (PIO_INPUT, RADIO_DIP1_PIO);

    // Initialise sysclock
    sysclock_init();

    //Initialiase Motors
    init_motors();

    // Initialise Radio
    init_radio();

    // Initialise tasks
    add_task(&toggle_status_led, STATUS_LED_BLINK_RATE);
    add_task(&communicate, SERIAL_POLL_RATE);

}


int
main (void)
{
    
    init();
    run_scheduler();
}
