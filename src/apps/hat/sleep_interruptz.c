#include "target.h"
#include "mcu_sleep.h"
#include "config.h"
#include "mcu.h"
#include "cpu.h"
#include "irq.h"

//Button includes:
#include "pio.h"
#include "pacer.h"
#include "button.h"


#define BUTTON_POLL_RATE 100

button_t sleep_button;

/* Define button configuration.  */
static const button_cfg_t sleep_button_cfg =
{
    .pio = SLEEP_BUTTON_PIO
};


//Configures the pin used for waking up
static mcu_sleep_wakeup_cfg_t wkup_config =
{
    .pio = SLEEP_BUTTON_PIO, //pin for wakeup
    .active_high = false // Wakeup on high signal = false
};


// Configures how deep asleep to go
static mcu_sleep_cfg_t sleep_config =
{
    .mode = MCU_SLEEP_MODE_WAIT //this can be changed to how deep asleep it goes?
};


//checks wakeup pin and puts mcu to sleep
void enter_sleep_mode(void)
{
    if (!mcu_sleep_wakeup_set(&wkup_config))
    {
        printf("uhoh no wakeup set");
        return;
    }
    mcu_sleep(&sleep_config);
}
/* Exit from backup mode occurs if there is an event on the
    WKUPEN0-15 pins, supply monitor (SM), RTC alarm, or RTT
    alarm.  The supply monitor monitors the voltage on the
    VDDIO pin if it is enabled.  The MCU is reset.  */

void init_sleeper(void) {
    sleep_button = button_init (&sleep_button_cfg);
}

void sleeper_stuff (void)
{
    // button_poll_count_set (BUTTON_POLL_COUNT (BUTTON_POLL_RATE));
    // pacer_init (BUTTON_POLL_RATE);

    button_poll (sleep_button);
    if (button_pushed_p (sleep_button))
    {
        enter_sleep_mode();
    }
}

