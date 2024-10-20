# Principal Makefile

##############################################
################## PROJECT ###################
##############################################

# Bootloader Version
VERSION			= v0.2.0

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
	@echo $(CONFIG_FILE)
