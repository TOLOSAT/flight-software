# Principal Makefile

##############################################
################## PROJECT ###################
##############################################

PROJ_NAME 		= boot-software
VERSION 		?= debug
BOARD 			?= ART_PI
FS_MODE			?= SDMMC

##############################################
################### MAKE #####################
##############################################

.PHONY = all clean echoes

all : clean build upload

include gen/settings.mk
include gen/path.mk
include gen/build.mk
include gen/debug.mk
include gen/code_verification.mk

clean :
	rm -rf $(BUILD_DIR)
