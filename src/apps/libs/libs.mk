THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
PERIPHERALS += adc
DRIVERS += ledtape
SRC += scheduler.c led_strip_blink.c battery_detect.c circular_buffer.c
VPATH += $(THIS_DIR)
INCLUDES += -I$(THIS_DIR)