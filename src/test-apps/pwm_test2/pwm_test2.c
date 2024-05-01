/* File:   pwm_test2.c
   Author: M. P. Hayes, UCECE
   Date:   15 April 2013
   Descr:  This example starts two channels simultaneously; one inverted
           with respect to the other.
*/
#include "pwm.h"
#include "pio.h"
#include "delay.h"
#include "panic.h"
#include "target.h"
#include "sam4s/sysclock/sysclock.h"

// If you are using PWM to drive a motor you will need
// to choose a lower frequency!
#define PWM_FREQ_HZ 1000

static pwm_cfg_t pwm1_cfg =
{
    .pio = MOTOR_A1_PIO,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 50),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_LOW,
    .stop_state = PIO_OUTPUT_LOW
}; 

static pwm_cfg_t pwm2_cfg =
{
    .pio = MOTOR_A2_PIO,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 0),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_LOW,
    .stop_state = PIO_OUTPUT_LOW
};

static pwm_cfg_t pwm3_cfg =
{
    .pio = MOTOR_B1_PIO,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 50),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_LOW,
    .stop_state = PIO_OUTPUT_LOW
};

static pwm_cfg_t pwm4_cfg =
{
    .pio = MOTOR_B2_PIO,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 0),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_LOW,
    .stop_state = PIO_OUTPUT_LOW
};

int
main (void)
{
    sysclock_init();
    pwm_t pwm1;
    pwm_t pwm2;
    pwm_t pwm3;
    pwm_t pwm4;

    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(BATTERY_DETECT_PIO, PIO_INPUT);
    pio_config_set(LED_USB_PIO, PIO_INPUT);

    pwm1 = pwm_init (&pwm1_cfg);
    if (! pwm1)
        panic (LED_ERROR_PIO, 1);

    pwm2 = pwm_init (&pwm2_cfg);
    if (! pwm2)
        panic (LED_ERROR_PIO, 2);

    pwm3 = pwm_init (&pwm3_cfg);
    if (! pwm3)
        panic (LED_ERROR_PIO, 3);
    
    pwm4 = pwm_init (&pwm4_cfg);
    if (!pwm4)
        panic (LED_STATUS_PIO, 4);

    pwm_channels_start (pwm_channel_mask (pwm1) | pwm_channel_mask (pwm2));

    pwm_channels_start (pwm_channel_mask (pwm3) | pwm_channel_mask (pwm4));

    while (1)
    {
        sysclock_millis_delay (5000);
        pwm_duty_set(pwm1, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, 0));
        pwm_duty_set(pwm3, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, 0));

        sysclock_millis_delay (5000);
        pwm_duty_set(pwm2, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, 50));
        pwm_duty_set(pwm4, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, 50));

        sysclock_millis_delay (5000);
        pwm_duty_set(pwm2, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, 0));
        pwm_duty_set(pwm4, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, 0));

        sysclock_millis_delay (5000);
        pwm_duty_set(pwm1, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, 50));
        pwm_duty_set(pwm3, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, 50));

    }

    return 0;
}
