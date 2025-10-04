# Kernel Makefile

##############################################
################ CONFIGURATION ###############
##############################################

# Kernel Version
MAJOR = 2
MINOR = 4
PATCH = 1

##############################################
################### MAKE #####################
##############################################

# Main recipe
all : pre-build hal fatfs freertos kernel

clean : kernel-clean

echoes :
	@echo "TAPAS"

##############################################
################## INCLUDES ##################
##############################################

include gen/paths.mk
include gen/configuration.mk
include gen/pre-build.mk
include gen/build-hal.mk
include gen/build-fatfs.mk
include gen/build-freertos.mk
include gen/build-kernel.mk
include gen/verification.mk
include gen/help.mk
