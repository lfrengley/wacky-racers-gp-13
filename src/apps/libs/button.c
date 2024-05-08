// /* File:   button_test2.c
//    Author: M. P. Hayes, UCECE
//    Date:   3 May 2021
//    Descr:  Simple button test demo using debouncing
// */
// #include "mcu.h"
// #include "pio.h"
// #include "pacer.h"
// #include "button.h"

// #define BUTTON_POLL_RATE 100
    
// static button_t button1;

// /* Define button configuration.  */
// static const button_cfg_t button1_cfg =
// {
//     .pio = SLEEP_BUTTON_PIO
// };

// /*Initialise the button*/
// void init_sleep_butt (void)
// {
    
//     button1 = button_init (&button1_cfg);

//     button_poll_count_set (BUTTON_POLL_COUNT (BUTTON_POLL_RATE));

//     pacer_init (BUTTON_POLL_RATE);

// }

// /* Todo: Configure an interrupt to change MCU mode from sleep to active when button is pressed*/
// void check_sleep_butt (void)
// {
//     button_poll (button1);

//     if (button_pushed_p (button1))
//     {
//         //go to sleep 
//         // or wake up
//     }
// }




// int main (void)
// {

//     /* Turn on LED.  */
//     led_set (led1, 1);
//     /* Initialise button.  */
//     while (1)
//     {
//         pacer_wait ();

//         button_poll (button1);

//         if (button_pushed_p (button1))
//         {
//             led_toggle (led1);
//         }
//     }
//     return 0;
// }
