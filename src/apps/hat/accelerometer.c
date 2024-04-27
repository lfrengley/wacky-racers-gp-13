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

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} AccelVector;

typedef struct {
    AccelVector samples[NUM_SAMPELS];
    uint8_t index = 0;
} MovingAverageFilter;

static MovingAverageFilter filter;

static twi_cfg_t adxl345_twi_cfg =
{
    .channel = TWI_CHANNEL_0,
    .period = TWI_PERIOD_DIVISOR (100000), // 100 kHz
    .slave_addr = 0
};

static void init_moving_average_filter(MovingAverageFilter *filter) {
    for (uint8_t i = 0; i < NUM_SAMPLES; i++) {
        filter->samples[i].x = 0;
        filter->samples[i].y = 0;
        filter->samples[i].z = 0;
    }
    filter->index = 0;
}

static void write_accel_vector(AccelVector *accel, int16_t accel_array[3]) {
    accel->x = accel_array[0];
    accel->y = accel_array[1];
    accel->z = accel_array[2];
}

static void calculate_percent_gravity(AccelVector *avg_raw_accel, AccelVector *accel_gravity_perc) {
    /* 
    Takes raw accelerometer data, and converts it to percent gravity using
    the range of the accelerometer output (+-Range ()) and the resulotion 
    of the accelerometer output in bits (N)
    */

    int16_t resolution = pow(2, N-1);

    // acceleration as a percentage of gravity 
    accel_gravity_perc->x = (avg_raw_accel->x * 100 /resolution) * RANGE;
    accel_gravity_perc->y = (avg_raw_accel->y * 100 /resolution) * RANGE;
    accel_gravity_perc->z = (avg_raw_accel->z * 100 /resolution) * RANGE;
}

static void calculate_pitch_roll(AccelVector *accel_gravity_perc, int32_t *pitch, int32_t *roll) {

    int16_t x = accel_gravity_perc->x;
    int16_t y = accel_gravity_perc->y;
    int16_t z = accel_gravity_perc->z;

    *pitch = (int32_t)((asinf(x / sqrtf((float)(x*x + y*y + z*z))) * TO_DEG));
    *roll = (int32_t)((atan2f(y, z) * TO_DEG));
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

    init_moving_average_filter(&filter);
}

static void update_moving_average_filter(MovingAverageFilter *filter, AccelVector *raw_accel) {
    filter->samples[filter->index].x = raw_accel->x;
    filter->samples[filter->index].y = raw_accel->y;
    filter->samples[filter->index].z = raw_accel->z;

    filter->index++;
    if (filter->index >= NUM_SAMPLES) {
        filter->index = 0;
    }
}

static void calc_moving_average(const MovingAverageFilter *filter, AccelVector *avg_accel) {
    int32_t sum_x = 0, sum_y = 0, sum_z = 0;

    for (uint8_t i = 0; i < NUM_SAMPLES; i++) {
        sum_x += filter->samples[i].x;
        sum_y += filter->samples[i].y;
        sum_z += filter->samples[i].z;
    }

    avg_accel->x = sum_x / NUM_SAMPLES;
    avg_accel->y = sum_y / NUM_SAMPLES;
    avg_accel->z = sum_z / NUM_SAMPLES;
}

bool check_accelerometer(void) {
        /* Read in the accelerometer data.  */
        int16_t accel_array[3];
        AccelVector raw_accel;
        AccelVector avg_raw_accel;
        AccelVector accel_gravity_perc;
        int32_t pitch, roll;

        bool accelerometer_state;
        if (! adxl345_is_ready (adxl345)) {
            printf ("Waiting for accelerometer to be ready...\n");
            return false;
        }

        if (adxl345_accel_read (adxl345, accel_array)) {
            // Filter the raw accelerometer data
            write_accel_vector(&raw_accel, accel_array);
            update_moving_average_filter(&filter, &raw_accel);
            calc_moving_average(&filter, &avg_raw_accel);

            // Convert raw values to pitch and roll
            calculate_percent_gravity(&avg_raw_accel, &accel_gravity_perc);
            calculate_pitch_roll(&accel_gravity_perc, &pitch, &roll);

            // printf ("x: %5d  y: %5d  z: %5d\n", accel_gravity_perc.x, accel_gravity_perc.y, accel_gravity_perc.z); 
            printf ("pitch: %3ld deg\troll %3ld deg\n", pitch/1000, roll/1000); //pitch doesn't go full 180?
            
            return true;
        } else {
            printf ("ERROR: failed to read acceleration\n");
            return false;
        }
}

void set_duty(uint8_t *duty1, uint8_t *duty2, uint8_t *duty3, uint8_t *duty4) {
    *duty1 = 1;
    *duty2 = 2;
    *duty3 = 3;
    *duty4 = 4;

}