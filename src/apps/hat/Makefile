PERIPHERALS = pit sysclock spi
DRIVERS = pacer usb_serial adxl345 panic piezo nrf24 button
SRC = hat.c accelerometer.c radio.c sleep_interruptz.c

# Use low level of optimisation to simplify debugging
OPT = -O1

BOARD = hat

include ../libs/libs.mk
include ../../boards/board.mk
