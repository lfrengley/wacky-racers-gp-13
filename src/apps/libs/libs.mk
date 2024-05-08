THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
PERIPHERALS += spi
DRIVERS += nrf24
SRC += scheduler.c radio.c button.c
VPATH += $(THIS_DIR)
INCLUDES += -I$(THIS_DIR)