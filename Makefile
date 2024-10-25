# Principal Makefile

##############################################
################ CONFIGURATION ###############
##############################################

# Software Version
VERSION			= v2.0.0

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
