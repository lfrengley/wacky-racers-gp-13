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
#include "radio.h"

#define PACER_RATE 20
#define SERIAL_POLL_RATE 1
#define STATUS_LED_BLINK_RATE 100

void toggle_status_led(void) {
    pio_output_toggle (LED_STATUS_PIO);
    // printf("Toggling Status LED\n");
}

void scan_serial(void) {
    int16_t duty_left, duty_right;
    char buf[256];
    radio_read_duties(&duty_left, &duty_right);
    set_motor_duties(duty_left, duty_right);
    // if (fgets(buf, sizeof(buf), stdin)) {
    //     // sscanf returns the number of input items successfully matched
    //     if (sscanf(buf, "%hd %hd",&duty_left, &duty_right) == 2) {
    //         set_motor_duties(duty_left, duty_right);
    //         printf ("Left Duty: %3d%%, \tRight Duty: %3d%%\n\n", duty_left, duty_right);
    //     } else {
    //         printf("Invalid input\n");
    //     }
    // }
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

    //Initialiase Motors
    init_motors();

    // Initialise Radio
    init_radio();

    // Initialise tasks
    add_task(&toggle_status_led, STATUS_LED_BLINK_RATE);
    add_task(&scan_serial, SERIAL_POLL_RATE);

}


int
main (void)
{
    
    init();
    run_scheduler();
}
