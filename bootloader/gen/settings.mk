# Makefile including all environnement parameters

include gen/conf_boards/$(BOARD).mk 
include gen/cc_settings.mk

##############################################
################# ENVIRONMENT ################
##############################################

# Export the variable DOCKER_WARNING like that the message appear only once per make call
export DOCKER_WARNING = no

# Checks if the code is executed inside a docker container
ifneq ($(MAKECMDGOALS), verif)
ifneq ($(shell echo $$DOCKER_WARNING), no)
$(warning *************************************************************)
$(warning ***** Not inside the docker. Environment is deprecated. *****)
$(warning *****        Program will starts in few seconds.        *****)
$(warning *************************************************************)
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
GDB     = arm-none-eabi-gdb
OCD     = openocd
CHECKER = cppcheck

CC_TARGETED_VERSION = 10.3.1
CC_VERSION = $(shell $(CC) -dumpversion)

ifneq ($(MAKECMDGOALS), verif)
ifneq ($(CC_VERSION), $(CC_TARGETED_VERSION))
$(error Wrong compiler is installed. arm-none-eabi-gcc v10.3.1 is required)
endif
endif

CHECKER_TARGETED_VERSION = 2.7
CHECKER_VERSION = $(shell $(CHECKER) --version | sed 's/[^0-9.]*\([0-9.]*\).*/\1/')

ifeq ($(MAKECMDGOALS), verif)
ifneq ($(CHECKER_VERSION), $(CHECKER_TARGETED_VERSION))
$(error Wrong code analyser is installed. cppcheck 2.7 is required)
endif
endif

##############################################
################# FS SETTINGS ################
##############################################

# FS_MODE validation
VALID_FS_MODES = SDMMC
ifneq ($(filter $(FS_MODE),$(VALID_FS_MODES)),)
# If FS_MODE is valid, nothing to do
else
$(error Bootloader is only compatible with SDMMC file system)
endif