# Principal Makefile

##############################################
################ CONFIGURATION ###############
##############################################

# Software Version
MAJOR = 2
MINOR = 2
PATCH = 6

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
