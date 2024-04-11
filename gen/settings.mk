# Makefile including all environnement parameters

include gen/board_settings.mk 
include gen/cc_settings.mk

##############################################
################# ENVIRONMENT ################
##############################################

# Docker Warning Goals Execptions 
DOCKER_WARNING_EXECEPTIONS = verif config

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

# Number of processor in order to improve speed of compilation
NUM_PROCESSORS = $(shell nproc)

##############################################
################### TOOLS ####################
##############################################

# Tools
CC      = $(shell which arm-none-eabi-gcc)
AR      = $(shell which arm-none-eabi-ar)
SIZE    = $(shell which arm-none-eabi-size)
READELF = $(shell which arm-none-eabi-readelf)
STRIP   = $(shell which arm-none-eabi-strip)
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

# LOAD_MEMORY validation
VALID_LOAD_MEMORY = flash ram
ifneq ($(filter $(LOAD_MEMORY),$(VALID_LOAD_MEMORY)),)
# If LOAD_MEMORY is valid, nothing to do
else
$(error Load memory can only be flash or ram)
endif

##############################################
############### Console SETTINGS ##############
##############################################

# CONSOLE_MODE validation
VALID_CONSOLE_MODES = none uart file circular-buffer
ifneq ($(filter $(CONSOLE_MODE),$(VALID_CONSOLE_MODES)),)
# If CONSOLE_MODE is valid, nothing to do
else
$(error Console mode can only be none, uart, file, or circular-buffer)
endif

##############################################
################# FS SETTINGS ################
##############################################

# FS_MODE validation
VALID_FS_MODES = none spi sdmmc
ifneq ($(filter $(FS_MODE),$(VALID_FS_MODES)),)
# If FS_MODE is valid, nothing to do
else
$(error File system mode can only be none or spi)
endif