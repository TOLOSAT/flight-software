# Makefile incluant les parametres d'envirronement

include conf/board_settings.mk 
include conf/cc_settings.mk

##############################################
################### TOOLS ####################
##############################################

# Tools
CC   = $(shell which arm-none-eabi-gcc)
AR   = $(shell which arm-none-eabi-ar)
SIZE = $(shell which arm-none-eabi-size)
GDB  = $(shell which gdb-multiarch || which gdb)
OCD  = $(shell which openocd)

CC_TARGETED_VERSION = 10.3.1
CC_VERSION = $(shell $(CC) -dumpversion)

ifneq ($(CC_VERSION), $(CC_TARGETED_VERSION))
	print := $(error Wrong Version of the compiler is installed. arm-none-eabi-gcc v10.3.1 is required)
endif