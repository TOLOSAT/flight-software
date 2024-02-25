# Makefile including all environnement parameters

include gen/board_settings.mk 
include gen/cc_settings.mk

##############################################
################# ENVIRONMENT ################
##############################################

# Checks if the code is executed inside a docker container
ifneq ($(MAKECMDGOALS), verif)
ifneq ($(shell echo $$IS_A_DOCKER), yes)
$(warning *************************************************************)
$(warning ***** Not inside the docker. Environment is deprecated. *****)
$(warning *****        Program will starts in few seconds.        *****)
$(warning *************************************************************)
do := $(shell sleep 3)
endif
endif

##############################################
################### TOOLS ####################
##############################################

# Tools
CC      = $(shell which arm-none-eabi-gcc)
AR      = $(shell which arm-none-eabi-ar)
SIZE    = $(shell which arm-none-eabi-size)
READELF = $(shell which arm-none-eabi-readelf)
GDB     = $(shell which gdb-multiarch || which gdb)
OCD     = $(shell which openocd)
CHECKER = $(shell which cppcheck)
PYTHON  = $(shell which python3)

CC_TARGETED_VERSION = 10.3.1
CC_VERSION = $(shell $(CC) -dumpversion)

ifneq ($(MAKECMDGOALS), verif)
ifneq ($(CC_VERSION), $(CC_TARGETED_VERSION))
$(error Wrong Version of the compiler is installed. arm-none-eabi-gcc v10.3.1 is required)
endif
endif

##############################################
############### MEMORY SETTINGS ##############
##############################################

ifeq ($(LOAD_MEMORY), flash)
LOAD_TYPE = LOAD_FLASH
else ifeq ($(LOAD_MEMORY), ram)
LOAD_TYPE = LOAD_RAM
else
$(error Load memory can only be flash or ram)
endif

##############################################
############### Console SETTINGS ##############
##############################################

ifeq ($(CONSOLE_MODE), none)
CONSOLE_TYPE = CONSOLE_NONE
else ifeq ($(CONSOLE_MODE), uart)
CONSOLE_TYPE = CONSOLE_UART
else ifeq ($(CONSOLE_MODE), fs)
CONSOLE_TYPE = CONSOLE_FS
else ifeq ($(CONSOLE_MODE), circular-buffer)
CONSOLE_TYPE = CONSOLE_CIRCULAR_BUFFER
else
$(error Console mode can only be none, uart, file or circular-buffer)
endif
