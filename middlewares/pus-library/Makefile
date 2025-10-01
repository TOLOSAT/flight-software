# PUS library Makefile

##############################################
################# PARAMETERS #################
##############################################

TOOLCHAIN 		  	?= $(error TOOLCHAIN is required)
CFLAGS 	  			?= $(error CFLAGS is required)
KERNEL_HEADERS		?= $(error KERNEL_HEADERS is required)

##############################################
################### MAKE #####################
##############################################

LIB_NAME = pus

# Main recipe
all : build

##############################################
################## INCLUDES ##################
##############################################

include gen/path.mk
include gen/settings.mk
include gen/build.mk