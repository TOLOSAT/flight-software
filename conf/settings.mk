# Makefile incluant les parametres d'envirronement

include conf/board_settings.mk 
include conf/cc_settings.mk

##############################################
################### TOOLS ####################
##############################################

# Tools
CC   = /usr/bin/arm-none-eabi-gcc
AR   = /usr/bin/arm-none-eabi-ar
SIZE = /usr/bin/arm-none-eabi-size
GDB  = /usr/bin/gdb-multiarch
OCD  = /usr/bin/openocd

CC_TARGETED_VERSION = 10.3.1
CC_VERSION = $(shell $(CC) -dumpversion)

ifneq ($(CC_VERSION), $(CC_TARGETED_VERSION))
	print := $(error Wrong Version of the compiler is installed. arm-none-eabi-gcc v10.3.1 is required)
endif