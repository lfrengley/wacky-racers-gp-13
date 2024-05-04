/** @file   motors.c
    @author Lucy Frengley
            Hayden Evans
            Penelope Keeling
            Michael Spurdle
            M. P. Hayes
    @date   4 May 2024
    @brief  TODO: Write a description
*/

#include "motors.h"
#include "pwm.h"
#include "pio.h"
#include "target.h"
#include <stdlib.h>

#define PWM_FREQ_HZ 1000
static pwm_t PWM1;
static pwm_t PWM2;
static pwm_t PWM3;
static pwm_t PWM4;

static pwm_cfg_t pwm1_cfg =
{
    .pio = MOTOR_A1_PIO,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 0),
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
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 0),
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

void init_motors (void) {
    PWM1 = pwm_init (&pwm1_cfg);
    if (! PWM1) 
        panic (LED_ERROR_PIO, 1);

    PWM2 = pwm_init (&pwm2_cfg);
    if (! PWM1)
        panic (LED_ERROR_PIO, 2);

    PWM3 = pwm_init (&pwm3_cfg);
    if (! PWM1)
        panic (LED_ERROR_PIO, 3);
    
    PWM4 = pwm_init (&pwm4_cfg);
    if (!PWM3)
        panic (LED_STATUS_PIO, 4);

    pwm_channels_start (pwm_channel_mask (PWM1) | pwm_channel_mask (PWM2));
    pwm_channels_start (pwm_channel_mask (PWM3) | pwm_channel_mask (PWM4));
}


void set_motor_duties (int16_t left, int16_t right) {

    if (left < 0) { // if reversing

        pwm_duty_set(PWM2, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, 0));
        pwm_duty_set(PWM4, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, 0));

        pwm_duty_set(PWM1, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, abs(left)));
        pwm_duty_set(PWM3, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, abs(right)));

    } else {
 
        pwm_duty_set(PWM1, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, 0));
        pwm_duty_set(PWM3, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, 0));

        pwm_duty_set(PWM2, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, left));
        pwm_duty_set(PWM4, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, right));
    }
}