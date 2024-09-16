# Makefile including all environnement parameters

##############################################
############## PROJECT SETTINGS ##############
##############################################

include .config

BOARD			?= ART_PI
LOAD_MEMORY		?= RAM
CONSOLE_MODE	?= FILE
FS_MODE			?= SD

# Project Name
PROJ_NAME	= $(patsubst ",,$(CONFIG_PROJ_NAME))

# Build Type (debug/release)
ifeq ($(CONFIG_BUILD_DEBUG), y)
VERSION_FLAGS 	= $(DEBUG_FLAGS)
BUILD_TYPE		= debug
else ifeq ($(CONFIG_BUILD_RELEASE), y)
VERSION_FLAGS 	= $(RELEASE_FLAGS)
BUILD_TYPE		= release
else
$(error Please select debug or release)
endif

##############################################
############# CONFIGURATION CHECK ############
##############################################

CONFIG_FILE_PRESENT := $(shell if [ -f .config ]; then echo "yes"; else echo "no"; fi)
CONFIG_WARNING_EXECEPTIONS = verif config menuconfig %_defconfig
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
############## ENVIRONMENT CHECK #############
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
############# CONF SETTINGS CHECK ############
##############################################

include gen/conf_boards/$(BOARD).mk 

# LOAD_MEMORY validation
ifneq ($(filter $(LOAD_MEMORY),$(VALID_LOAD_MEMORY)),)
# If LOAD_MEMORY is valid, nothing to do
else
$(error This load memory is not available for this board)
endif

# CONSOLE_MODE validation
ifneq ($(filter $(CONSOLE_MODE),$(VALID_CONSOLE_MODES)),)
# If CONSOLE_MODE is valid, nothing to do
else
$(error This console mode is not available for this board)
endif

# FS_MODE validation
ifneq ($(filter $(FS_MODE),$(VALID_FS_MODES)),)
# If FS_MODE is valid, nothing to do
else
$(error This file system mode is not available for this board)
endif
