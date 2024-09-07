# Makefile including all environnement parameters

##############################################
################ CONFIGURATION ###############
##############################################

CONFIG_FILE_PRESENT := $(shell if [ -f .config ]; then echo "yes"; else echo "no"; fi)
DOCKER_WARNING_EXECEPTIONS = verif config menuconfig
ifeq ($(filter $(CONFIG_WARNING_EXECEPTIONS),$(MAKECMDGOALS)),)
ifeq ($(CONFIG_FILE_PRESENT),no)
$(warning *************************************************************)
$(warning *****    No config file. Default configuration used.    *****)
$(warning *****        Program will starts in few seconds.        *****)
$(warning *************************************************************)
else
include .config
endif
endif

##############################################
################## INCLUDES ##################
##############################################

include gen/conf_boards/$(BOARD).mk 
include gen/cc_settings.mk

##############################################
################# ENVIRONMENT ################
##############################################

# Docker Warning Goals Execptions 
DOCKER_WARNING_EXECEPTIONS = verif config menuconfig

# Checks if the code is executed inside a docker container
ifeq ($(filter $(DOCKER_WARNING_EXECEPTIONS),$(MAKECMDGOALS)),)
ifneq ($(shell echo $$DOCKER_WARNING), no)
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
CC      = arm-none-eabi-gcc
AR      = arm-none-eabi-ar
SIZE    = arm-none-eabi-size
READELF = arm-none-eabi-readelf
STRIP   = arm-none-eabi-strip
GDB     = arm-none-eabi-gdb
EMU		= qemu-system-arm
OCD     = openocd
CHECKER = cppcheck
PYTHON  = python3
KCONF	= kconfig

CC_TARGETED_VERSION = 10.3.1
CC_VERSION = $(shell $(CC) -dumpversion)

ifneq ($(MAKECMDGOALS), verif)
ifneq ($(MAKECMDGOALS), conf-files)
ifneq ($(CC_VERSION), $(CC_TARGETED_VERSION))
$(error Wrong Version of the compiler is installed. arm-none-eabi-gcc v10.3.1 is required)
endif
endif
endif

##############################################
############### MEMORY SETTINGS ##############
##############################################

# LOAD_MEMORY validation
ifneq ($(filter $(LOAD_MEMORY),$(VALID_LOAD_MEMORY)),)
# If LOAD_MEMORY is valid, nothing to do
else
$(error This load memory is not available for this board)
endif

##############################################
############### Console SETTINGS ##############
##############################################

# CONSOLE_MODE validation
ifneq ($(filter $(CONSOLE_MODE),$(VALID_CONSOLE_MODES)),)
# If CONSOLE_MODE is valid, nothing to do
else
$(error This console mode is not available for this board)
endif

##############################################
################# FS SETTINGS ################
##############################################

# FS_MODE validation
ifneq ($(filter $(FS_MODE),$(VALID_FS_MODES)),)
# If FS_MODE is valid, nothing to do
else
$(error This file system mode is not available for this board)
endif