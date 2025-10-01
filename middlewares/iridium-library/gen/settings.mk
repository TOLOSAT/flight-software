# Makefile including all environnement parameters

ifndef SETTINGS_MK
SETTINGS_MK := yes

##############################################
################### TOOLS ####################
##############################################

CC = $(TOOLCHAIN)-gcc
AR = $(TOOLCHAIN)-ar

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

endif # SETTINGS_MK #