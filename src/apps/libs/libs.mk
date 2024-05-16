THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
DRIVERS += ledtape
SRC += scheduler.c led_strip_blink.c
VPATH += $(THIS_DIR)
INCLUDES += -I$(THIS_DIR)