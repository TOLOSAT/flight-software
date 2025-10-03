# Makefile including all environnement parameters

ifndef SETTINGS_MK
SETTINGS_MK := yes

##############################################
################### TOOLS ####################
##############################################

TOOLCHAIN 	= arm-none-eabi
CC      	= $(TOOLCHAIN)-gcc
AR      	= $(TOOLCHAIN)-ar
SIZE    	= $(TOOLCHAIN)-size
READELF 	= $(TOOLCHAIN)-readelf
NM			= $(TOOLCHAIN)-nm
STRIP   	= $(TOOLCHAIN)-strip
GDB     	= $(TOOLCHAIN)-gdb
EMU			= qemu-system-arm
OCD     	= openocd
CHECKER 	= cppcheck
PYTHON  	= python3
KCONF		= kconfig

##############################################
############ PROJECT CONFIGURATION ###########
##############################################

# Configuration files
CONFIG_FILE		= .config
OLD_CONFIG_FILE	= .config.old
DEFAULT_CONFIG 	= $(CONFIGS_DIR)/default_defconfig

# Configuration presence check
CONFIG_FILE_PRESENT = $(wildcard $(CONFIG_FILE))
CONFIG_WARNING_EXECEPTIONS = config format menuconfig %_defconfig
ifeq ($(CONFIG_FILE_PRESENT),)
ifeq ($(filter $(CONFIG_WARNING_EXECEPTIONS),$(MAKECMDGOALS)),)
$(warning *************************************************************)
$(warning *****               No config file found.               *****)
$(warning *****            Please load a configuration.           *****)
$(warning *************************************************************)
$(error )
endif
include $(DEFAULT_CONFIG)
else
include $(CONFIG_FILE)
endif

# Project Name
PROJ_NAME = $(subst ",,$(CONFIG_PROJ_NAME))

# Config Name
CONFIG_NAME = $(subst ",,$(CONFIG_CONFIG_NAME))

# Board and Chip Information
BOARD = $(subst ",,$(CONFIG_BOARD_NAME))
CHIP_FAMILLY = $(subst ",,$(CONFIG_CHIP_FAMILLY))
MACH = $(subst ",,$(CONFIG_ARCH))

# Optimisation
ifeq ($(CONFIG_OPT_O0),y)
CFLAGS_OPTIMISATION = -O0
endif
ifeq ($(CONFIG_OPT_O1),y)
CFLAGS_OPTIMISATION += -O1
endif
ifeq ($(CONFIG_OPT_O2),y)
CFLAGS_OPTIMISATION += -O2
endif
ifeq ($(CONFIG_OPT_O3),y)
CFLAGS_OPTIMISATION += -O3
endif
ifeq ($(CONFIG_OPT_OS),y)
CFLAGS_OPTIMISATION += -Os
endif

# Debug
ifeq ($(CONFIG_DEBUG_G0),y)
CFLAGS_DEBUG += -g0
endif
ifeq ($(CONFIG_DEBUG_G1),y)
CFLAGS_DEBUG += -g1
endif
ifeq ($(CONFIG_DEBUG_G2),y)
CFLAGS_DEBUG += -g2
endif
ifeq ($(CONFIG_DEBUG_G3),y)
CFLAGS_DEBUG += -g3
endif

# Load Memory (ram/flash)
ifeq ($(CONFIG_LOAD_MEMORY_RAM), y)
LOAD_MEMORY = ram
else
LOAD_MEMORY = flash
endif

# FPU configuration
ifeq ($(CONFIG_HAS_FPU), y)
FPU_SETTINGS = -mfpu=$(subst ",,$(CONFIG_FPU_TYPE)) -mfloat-abi=hard
else
FPU_SETTINGS = -mfloat-abi=soft
endif

# Test Selection
ifneq ($(CONFIG_TEST_NAME),)
TEST_NAME = $(subst ",,$(CONFIG_TEST_NAME))
APPLICATIONS_DIR = $(TESTS_DIR)/$(TEST_NAME)
endif

##############################################
############## ENVIRONMENT CHECK #############
##############################################

# Checks if the code is executed inside a docker container
DOCKER_WARNING_EXECEPTIONS = help clean upload debug gdb flash-erase set-boot-ram set-boot-flash
ifeq ($(filter $(DOCKER_WARNING_EXECEPTIONS),$(MAKECMDGOALS)),)
ifneq ($(shell echo $$DOCKER_WARNING), no)
$(warning *************************************************************)
$(warning ***** Not inside the docker. Environment is deprecated. *****)
$(warning *****        Program will starts in few seconds.        *****)
$(warning *************************************************************)
endif
endif

# Checks if the right compiler is used
CC_TARGETED_VERSION = 10.3.1
CC_VERSION = $(shell $(CC) -dumpversion)
COMPILER_WARNING_EXECEPTIONS = help clean \
	config menuconfig %_defconfig pre-build \
	upload debug gdb flash-erase set-boot-ram set-boot-flash \
	verif doc format
ifneq ($(findstring n, $(MAKEFLAGS)), n)
ifeq ($(filter $(COMPILER_WARNING_EXECEPTIONS),$(MAKECMDGOALS)),)
ifneq ($(CC_VERSION), $(CC_TARGETED_VERSION))
$(error Wrong compiler is installed. arm-none-eabi-gcc v10.3.1 is required)
endif
endif
endif

# Checks if the right code checker is used
CHECKER_TARGETED_VERSION = 2.7
CHECKER_VERSION = $(shell $(CHECKER) --version | sed 's/[^0-9.]*\([0-9.]*\).*/\1/')
ifeq ($(MAKECMDGOALS), verif)
ifneq ($(CHECKER_VERSION), $(CHECKER_TARGETED_VERSION))
$(error Wrong code analyser is installed. cppcheck 2.7 is required)
endif
endif

endif # SETTINGS_MK #