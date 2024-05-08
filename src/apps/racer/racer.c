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
#include "scheduler.h"

#define PACER_RATE 20
#define SERIAL_POLL_RATE 1
#define STATUS_LED_BLINK_RATE 100

void toggle_status_led(void) {
    pio_output_toggle (LED_STATUS_PIO);
    // printf("Toggling Status LED\n");
}

void scan_serial(void) {
    int16_t pwm_left, pwm_right;
    char buf[256];
    if (fgets(buf, sizeof(buf), stdin)) {
        // sscanf returns the number of input items successfully matched
        if (sscanf(buf, "%hd %hd",&pwm_left, &pwm_right) == 2) {
            set_motor_duties(pwm_left, pwm_right);
            printf("%hd\n",  pwm_left);
            printf("%hd\n", pwm_right);
        } else {
            printf("Invalid input\n");
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

    sysclock_init();
    add_task(&toggle_status_led, STATUS_LED_BLINK_RATE);
    add_task(&scan_serial, SERIAL_POLL_RATE);

    init_motors();
}


int
main (void)
{
    
    init();
    run_scheduler();
}
