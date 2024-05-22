THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
PERIPHERALS += adc
SRC += scheduler.c battery_detect.c circular_buffer.c
VPATH += $(THIS_DIR)
INCLUDES += -I$(THIS_DIR)