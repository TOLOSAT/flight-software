# Makefile including all environnement parameters

include conf/board_settings.mk 
include conf/cc_settings.mk

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

CC_TARGETED_VERSION = 10.3.1
CC_VERSION = $(shell $(CC) -dumpversion)

ifneq ($(MAKECMDGOALS), verif)
ifneq ($(CC_VERSION), $(CC_TARGETED_VERSION))
$(error Wrong Version of the compiler is installed. arm-none-eabi-gcc v10.3.1 is required)
endif
endif