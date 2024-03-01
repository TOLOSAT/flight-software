# Makefile including all environnement parameters

include gen/board_settings.mk 
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

# Validation de LOAD_MEMORY
VALID_LOAD_MEMORY = flash ram
ifneq ($(filter $(LOAD_MEMORY),$(VALID_LOAD_MEMORY)),)
# Si LOAD_MEMORY est valide, rien à faire ici
else
$(error Load memory can only be flash or ram)
endif

##############################################
############### Console SETTINGS ##############
##############################################

# Validation de CONSOLE_MODE
VALID_CONSOLE_MODES = none uart fs circular-buffer
ifneq ($(filter $(CONSOLE_MODE),$(VALID_CONSOLE_MODES)),)
# Si CONSOLE_MODE est valide, rien à faire ici
else
$(error Console mode can only be none, uart, fs, or circular-buffer)
endif

##############################################
################# FS SETTINGS ################
##############################################

# Validation de FS_MODE
VALID_FS_MODES = none spi sdmmc
ifneq ($(filter $(FS_MODE),$(VALID_FS_MODES)),)
# Si FS_MODE est valide, rien à faire ici
else
$(error File system mode can only be none or spi)
endif