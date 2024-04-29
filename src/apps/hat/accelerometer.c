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
    AccelVector samples[NUM_SAMPLES];
    uint8_t index;
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
    if (! adxl345_twi) {
        panic (LED_ERROR_PIO, 1);
    }

    // Initialise the ADXL345
    adxl345 = adxl345_init (adxl345_twi, ADXL345_ADDRESS);   
    if (! adxl345) {
        panic (LED_ERROR_PIO, 2);
    }

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

static int32_t clip_values(int32_t val, int32_t max_val, int32_t min_val) {
    if (val < min_val) {
        val = min_val;
    } else if (val > max_val) {
        val = max_val;
    }
    return val;
}

static void set_duty(MotorDuties *duties, int32_t pitch, int32_t roll) {
    /*
    Takes the pitch and roll angles (degrees * 1000) of the accelerometer.
    Clips the values within accepetable range.
    Converts these to an abitrary forward and turning speed using the pitch
    and roll angles, and forward and turning gains. Adds a deadband in the middle
    where there is no speed.
    Divides these speeds by the max possible speeds to get a duty cycle percentage
    TODO: Check it actually works...
    */    
    int32_t ANGLE_MULTIPLIER = 1000; // for working with ints not floats
    int32_t max_pitch = 60 * ANGLE_MULTIPLIER; //degrees
    int32_t min_pitch = -max_pitch; //degrees
    int32_t pitch_deadband = 15 * ANGLE_MULTIPLIER;
    int32_t max_roll = 50 * ANGLE_MULTIPLIER;
    int32_t min_roll = -max_roll;
    int32_t roll_deadband = 15 * ANGLE_MULTIPLIER;

    // Calculate forward speed
    int32_t forward_speed = 0;
    int8_t speed_gain = 15; // random number

    if (abs(pitch) > pitch_deadband) {
        int32_t clipped_pitch = clip_values(pitch, max_pitch, min_pitch);
        printf("Clipped Pitch: %3ld", clipped_pitch);
        forward_speed = clipped_pitch * speed_gain;
    }
    // Calculate turning speed
    int32_t turning_speed = 0;
    int8_t turning_gain = 3;
    if (abs(roll) > roll_deadband) {
        int32_t clipped_roll = clip_values(roll, max_roll, min_roll);   
        printf("Clipped Roll: %3ld", clipped_roll);
        turning_speed = clipped_roll * turning_gain;
    }
    printf("Forward Speed: %3ld, Turning speed: %3ld", forward_speed, turning_speed);

    // Calculate motor speeds
    int32_t left_speed = forward_speed - turning_speed;
    int32_t right_speed = forward_speed + turning_speed;
    int32_t max_speed = max_pitch * speed_gain + max_roll * turning_gain;
    printf("Left Speed: %3ld, Right Speed: %3ld, Max Speed %3ld", left_speed, right_speed, max_speed);
    // Calculate duty cycles (may need to flip some of these around)
    duties->left = (left_speed * 100) / max_speed;
    duties->right = (right_speed * 100) / max_speed;
}

bool check_accelerometer(MotorDuties *duties) {
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
        
        set_duty(&duties, pitch, roll);

        return true;
    } else {
        printf ("ERROR: failed to read acceleration\n");
        return false;
    }
}

