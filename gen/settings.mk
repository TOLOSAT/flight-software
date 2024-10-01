# Makefile including all environnement parameters

ifndef SETTINGS_MK
SETTINGS_MK := yes

##############################################
############ PROJECT CONFIGURATION ###########
##############################################

# Configuration files
CONFIG_FILE		= .config
OLD_CONFIG_FILE	= .config.old
DEFAULT_CONFIG 	= configs/default_defconfig

# Configuration presence check
CONFIG_FILE_PRESENT = $(wildcard $(CONFIG_FILE))
CONFIG_WARNING_EXECEPTIONS = config menuconfig %_defconfig
ifeq ($(CONFIG_FILE_PRESENT),)
ifeq ($(filter $(CONFIG_WARNING_EXECEPTIONS),$(MAKECMDGOALS)),)
$(warning *************************************************************)
$(warning *****               No config file found.               *****)
$(warning *****        Default configuration will be used.        *****)
$(warning *************************************************************)
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
CHIP_VENDOR = $(subst ",,$(CONFIG_CHIP_VENDOR))
CHIP_FAMILLY = $(subst ",,$(CONFIG_CHIP_FAMILLY))
CHIP = $(subst ",,$(CONFIG_CHIP))
MACH = $(subst ",,$(CONFIG_ARCH))
ifdef CONFIG_DUAL_CORE
CORE_SELECT = -D$(subst ",,$(CONFIG_CORE_SELECT))
endif

# Build Type (debug/release)
ifeq ($(CONFIG_BUILD_DEBUG), y)
VERSION_FLAGS = $(DEBUG_FLAGS)
BUILD_TYPE = debug
else
VERSION_FLAGS = $(RELEASE_FLAGS)
BUILD_TYPE = release
endif

# Load Memory (ram/flash)
ifeq ($(CONFIG_LOAD_MEMORY_RAM), y)
LOAD_MEMORY = ram
else
LOAD_MEMORY = flash
endif

# FPU configuration
ifeq ($(CONFIG_FPU), y)
FPU_SETTINGS = -mfpu=$(subst ",,$(CONFIG_FPU_TYPE)) -mfloat-abi=hard
else
FPU_SETTINGS = -mfloat-abi=soft
endif

# Test Selection
ifneq ($(CONFIG_TEST_NAME),)
APPLICATION_DIR = $(TESTS_DIR)/$(subst ",,$(CONFIG_TEST_NAME))
endif

# Select FreeRTOS port
ifeq ($(CONFIG_ARCH),"cortex-m4")
ifeq ($(CONFIG_FPU),y)
FREERTOS_PORTABLE = ARM_CM4F
else
FREERTOS_PORTABLE = ARM_CM3
endif
else ifeq ($(CONFIG_ARCH),"cortex-m7")
ifeq ($(CONFIG_FPU),y)
FREERTOS_PORTABLE = ARM_CM4F
else
FREERTOS_PORTABLE = ARM_CM3
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

COMPILER_WARNING_EXECEPTIONS = verif autoconf conf-files
ifeq ($(filter $(COMPILER_WARNING_EXECEPTIONS),$(MAKECMDGOALS)),)
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

endif # SETTINGS_MK #