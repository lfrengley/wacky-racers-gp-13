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

/* Initialise */
void init(void) {
    // Redirect stdio to USB serial
    usb_serial_stdio_init ();

    // Initialise PIO pins
    pio_config_set (LED_ERROR_PIO, PIO_OUTPUT_LOW);
    pio_output_set (LED_ERROR_PIO, ! LED_ACTIVE);
    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_LOW);
    pio_output_set (LED_STATUS_PIO, ! LED_ACTIVE);
    
    init_motors();
}


int
main (void)
{
    int16_t pwm_left, pwm_right;
    init();

    while (1) {

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
}
