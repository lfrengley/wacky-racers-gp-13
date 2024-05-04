/**
 * A very simple USB serial calculator program.
 *
 * Supports addition:
 *
 *   [in]  1 + 2
 *   [out] 3
 *
 * and subtraction
 *
 *   [in]  4 - 12
 *   [out] -8
 *
 * There must be a single space either side of the operator.
 */

#include <stdio.h>
#include <string.h>
#include "usb_serial.h"
#include "pio.h"
#include "delay.h"
#include "panic.h"

#define DELAY_MS 10

int main (void)
{
    int i = 0;

    // Redirect stdio to USB serial
    if (usb_serial_stdio_init () < 0)
        panic(LED_ERROR_PIO, 3);

    while (1)
    {
        delay_ms (DELAY_MS);
        char buf[256];
        if (fgets(buf, sizeof(buf), stdin)) {
            int pwm1, pwm2;
            char operator;
            // sscanf returns the number of input items successfully matched
            if (sscanf(buf, "%d %d",&pwm1, &pwm2) == 2) {
                printf("%d\n",  pwm1);
                printf("%d\n", pwm2);
            } else {
                printf("Invalid input\n");
            }
        }
    }
}
