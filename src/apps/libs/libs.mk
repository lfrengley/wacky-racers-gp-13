THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
SRC += scheduler.c
VPATH += $(THIS_DIR)
INCLUDES += -I$(THIS_DIR)