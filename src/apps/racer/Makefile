PERIPHERALS = pwm pit spi
DRIVERS = usb_serial panic nrf24 ledtape
SRC = racer.c motors.c radio.c led_strip_blink.c

# Use low level of optimisation to simplify debugging
OPT = -O1

BOARD = racer
include ../libs/libs.mk
include ../../boards/board.mk

