/** @file   battery_detect.c
    @author Lucy Frengley
            Hayden Evans
            Penelope Keeling
            Michael Spurdle
    @date   17 May 2024
    @brief  TODO: Write a description
*/

#include "target.h"
#include "pio.h"
#include "adc.h"
#include <stdio.h>
#include <stdlib.h>
#include "circular_buffer.h"

#define BUF_SIZE 12
#define ADC_RANGE 4095

static circBuf_t g_inBuffer;  
adc_t adc;
uint16_t current_adc;

static const adc_cfg_t adc_cfg =
{
    .bits = 12,
    .channels = BIT (ADC_CHANNEL_1),
    .trigger = ADC_TRIGGER_SW,
    .clock_speed_kHz = 1000
};

void init_battery_detect (void) {
    adc = adc_init (&adc_cfg);
    initCircBuf (&g_inBuffer, BUF_SIZE);
    if (! adc)
        panic (LED_ERROR_PIO, 1);
}

uint16_t calculate_battery_average (void)
{
    uint32_t sum;
    for (size_t i = 0; i < BUF_SIZE; i++) {
        sum = sum + readCircBuf (&g_inBuffer); 
    }
    current_adc = ((2 * sum  + BUF_SIZE) / 2 / BUF_SIZE);
    return current_adc;
}

void poll_charge_status (void) {
    uint16_t data[1];
    adc_read (adc, data, sizeof (data));
    writeCircBuf (&g_inBuffer, data[0]);
}