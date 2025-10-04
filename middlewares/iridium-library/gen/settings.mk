# Makefile including all environnement parameters

ifndef SETTINGS_MK
SETTINGS_MK := yes

##############################################
################## DEFAULTS ##################
##############################################

KERNEL_HEADERS	?= $(error KERNEL_HEADERS is required)

ifndef TOOLCHAIN
$(warning TOOLCHAIN not set — using default 'arm-none-eabi')
TOOLCHAIN = arm-none-eabi
endif

ifndef CFLAGS
$(warning CFLAGS not set — using defaults)
CFLAGS = -c \
		-mcpu=cortex-m7 \
		-std=gnu11 \
		-ffunction-sections \
		-fdata-sections \
		-Wall  -Wextra  -Werror  \
		-mfpu=fpv5-d16 -mfloat-abi=hard  \
		-mthumb  \
		-funwind-tables  \
		-MMD -MP  \
		-g3 -O0
endif

##############################################
################### TOOLS ####################
##############################################

CC 		= $(TOOLCHAIN)-gcc
AR 		= $(TOOLCHAIN)-ar
CHECKER	= cppcheck

##############################################
############## ENVIRONMENT CHECK #############
##############################################

# Checks if the code is executed inside a docker container
DOCKER_WARNING_EXECEPTIONS = help clean
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
COMPILER_WARNING_EXECEPTIONS = help clean verif doc format
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