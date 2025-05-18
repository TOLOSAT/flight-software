# Principal Makefile

##############################################
################## PROJECT ###################
##############################################

# Bootloader Version
MAJOR = 0
MINOR = 4
PATCH = 1

##############################################
################### MAKE #####################
##############################################

.PHONY = all clean echoes

all : build

clean : build-clean

include gen/build.mk
include gen/debug.mk
include gen/configuration.mk
include gen/verification.mk
include gen/help.mk

echoes :
	@echo $(PROJ_NAME)
