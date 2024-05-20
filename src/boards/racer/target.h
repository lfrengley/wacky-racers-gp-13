/** @file   target.h
    @author M. P. Hayes, UCECE
    @date   12 February 2018
    @brief
*/
#ifndef TARGET_H
#define TARGET_H

#include "mat91lib.h"

/* This is for the carhat (chart) board configured as a racer!  */

/* System clocks  */
#define F_XTAL 12.00e6
#define MCU_PLL_MUL 16
#define MCU_PLL_DIV 1

#define MCU_USB_DIV 2
/* 192 MHz  */
#define F_PLL (F_XTAL / MCU_PLL_DIV * MCU_PLL_MUL)
/* 96 MHz  */
#define F_CPU (F_PLL / 2)

/* TWI  */
#define TWI_TIMEOUT_US_DEFAULT 10000

/* USB  */
//#define USB_VBUS_PIO PA5_PIO
#define USB_CURRENT_MA 500

/* LEDs  */
#define LED_STATUS_PIO PA8_PIO
#define LED_ERROR_PIO PA9_PIO
#define LED_USB_PIO PA31_PIO
#define LED_ACTIVE 0

/* General  */
#define GENERAL_PA28_PIO PA28_PIO
#define GENERAL_PA27_PIO PA27_PIO
#define GENERAL_PA25_PIO PA25_PIO
#define GENERAL_PA24_PIO PA24_PIO
#define GENERAL_PA23_PIO PA23_PIO
#define GENERAL_PA21_PIO PA21_PIO
#define GENERAL_PA20_PIO PA20_PIO
#define GENERAL_PA18_PIO PA18_PIO
#define GENERAL_PA17_PIO PA17_PIO
#define GENERAL_PA16_PIO PA16_PIO
#define SIGNAL_TP_PIO PA6_PIO

/* Bumper  */
#define BUMPER_POSITIVE PB13_PIO

/* Buttons  */
#define SLEEP_BUTTON_PIO PA0_PIO
#define ERASE_BUTTON_PIO PB12_PIO

/* H-bridges   */
#define MOTOR_A1_PIO PA7_PIO
#define MOTOR_A2_PIO PA11_PIO
#define MOTOR_B1_PIO PA1_PIO
#define MOTOR_B2_PIO PA2_PIO
#define MOTOR_FAULT_PIO PA30_PIO
#define MOTOR_SLEEP_PIO PA29_PIO

/* Radio  */
#define RADIO_ENABLE_PIO PA26_PIO
#define RADIO_DIP1_PIO PA22_PIO
#define RADIO_DIP2_PIO PA19_PIO
#define RADIO_CE_PIO PA15_PIO
#define RADIO_SCK_PIO PA14_PIO
#define RADIO_MOSI_PIO PA13_PIO
#define RADIO_MISO_PIO PA12_PIO
#define RADIO_CSN_PIO PA10_PIO
#define RADIO_IRQ_PIO PA4_PIO

/* Battery  */
#define BATTERY_DETECT_PIO ADC_CHANNEL_3

/* LED tape  */
#define LEDTAPE_PIO PA3_PIO

/* SWD  */
#define SWD_SWCLK_PIO PB7_PIO
#define SWD_SWDIO_PIO PB6_PIO

#endif /* TARGET_H  */
