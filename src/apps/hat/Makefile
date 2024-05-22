PERIPHERALS = pit sysclock spi pwm
DRIVERS = pacer usb_serial adxl345 panic nrf24 ledtape
SRC = hat.c accelerometer.c radio.c buzzer.c led_strip_blink.c

# Use low level of optimisation to simplify debugging
OPT = -O1

BOARD = hat

include ../libs/libs.mk
include ../../boards/board.mk
