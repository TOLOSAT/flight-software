# Kernel Makefile

##############################################
################# PARAMETERS #################
##############################################

TOOLCHAIN 		  	?= $(error TOOLCHAIN is required)
CFLAGS 	  			?= $(error CFLAGS is required)

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

##############################################
################## INCLUDES ##################
##############################################

include gen/path.mk
include gen/configuration.mk
include gen/pre-build.mk
include gen/third-parties.mk
include gen/build-kernel.mk