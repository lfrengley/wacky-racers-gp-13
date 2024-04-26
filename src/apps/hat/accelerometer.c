/** @file   accelerometer.c
    @author Lucy Frengley
            Hayden Evans
            Penelope Keeling
            Michael Spurdle
            M. P. Hayes
    @date   28 April 2024
    @brief  TODO: Write a description
*/
#include "accelerometer.h"
#include "adxl345.h"
#include "panic.h"

static int8_t RANGE = 2; // accelleration range in terms of g
static int8_t N = 10; // num bits at given range
static uint16_t TO_DEG = 57296; // 180 * 1000 / pi
static twi_t adxl345_twi;
static adxl345_t *adxl345;

static twi_cfg_t adxl345_twi_cfg =
{
    .channel = TWI_CHANNEL_0,
    .period = TWI_PERIOD_DIVISOR (100000), // 100 kHz
    .slave_addr = 0
};

static void calculate_percent_gravity(int16_t raw_accel[3], int32_t *ax, int32_t *ay, int32_t *az) {
    int16_t resolution = pow(2, N-1);

    // acceleration as a percentage of gravity 
    // (raw_accel * 100 /2^(n-1)) * abs(range)
    *ax = (raw_accel[0] * 100 /resolution) * RANGE;
    *ay = (raw_accel[1] * 100 /resolution) * RANGE;
    *az = (raw_accel[2] * 100 /resolution) * RANGE;
}

static void calculate_pitch_roll(int32_t ax, int32_t ay, int32_t az, int32_t *pitch, int32_t *roll) {
    // Calculate pitch angle
    *pitch = (int32_t)((asinf(ax / sqrtf((float)(ax * ax + ay * ay + az * az))) * TO_DEG));
    // Calculate roll angle
    *roll = (int32_t)((atan2f(ay, az) * TO_DEG));
}

void init_accelerometer(void) {
    // Initialise the TWI (I2C) bus for the ADXL345
    adxl345_twi = twi_init (&adxl345_twi_cfg);
    if (! adxl345_twi)
        panic (LED_ERROR_PIO, 1);

    // Initialise the ADXL345
    adxl345 = adxl345_init (adxl345_twi, ADXL345_ADDRESS);   
        if (! adxl345)
        panic (LED_ERROR_PIO, 2);
}

bool check_accelerometer(void) {
        /* Read in the accelerometer data.  */
        bool accelerometer_state;
        if (! adxl345_is_ready (adxl345)) {
            printf ("Waiting for accelerometer to be ready...\n");
            accelerometer_state = false;
        } else {
            int16_t accel[3];
            if (adxl345_accel_read (adxl345, accel)) {
                // Variables to store pitch and roll angles
                int32_t pitch, roll;
                int32_t ax, ay, az;
                
                calculate_percent_gravity(accel, &ax, &ay, &az);
                calculate_pitch_roll(ax, ay, az, &pitch, &roll);

                printf ("x: %5ld  y: %5ld  z: %5ld\n", ax, ay, az); 
                printf ("pitch: %3ld deg\troll %3ld deg\n", pitch/1000, roll/1000); //pitch doesn't go full 180?
                
                accelerometer_state = true;
            } else {
                printf ("ERROR: failed to read acceleration\n");
                accelerometer_state = false;
            }
        }

        return accelerometer_state;
}

void set_duty(uint8_t *duty1, uint8_t *duty2, uint8_t *duty3, uint8_t *duty4) {
    *duty1 = 1;
    *duty2 = 2;
    *duty3 = 3;
    *duty4 = 4;

}