# PUS Building Makefile

##############################################
################### OPTIONS ##################
##############################################

# Mandatory Options
TOOLCHAIN 		  	?= $(error TOOLCHAIN is required)
CFLAGS 	  			?= $(error CFLAGS is required)
KERNEL_HEADERS		?= $(error KERNEL_HEADERS is required)

# Optionnal Options
BUILD_DIR 			?= build
EXTRA_INCS			?=

##############################################
################### MAKE #####################
##############################################

LIB_NAME = pus

# Main recipe
all : build

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/build.mk