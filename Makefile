# Principal Makefile

##############################################
################ CONFIGURATION ###############
##############################################

# Software Version
VERSION			= v1.6.8

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

echoes :
	@echo $(PROJ_NAME)
