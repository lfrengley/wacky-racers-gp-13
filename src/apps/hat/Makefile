PERIPHERALS = pit sysclock
DRIVERS = pacer usb_serial adxl345 panic piezo
SRC = hat.c accelerometer.c ../libs/scheduler.c

# Use low level of optimisation to simplify debugging
OPT = -O1

BOARD = hat

include ../libs/libs.mk
include ../../boards/board.mk
