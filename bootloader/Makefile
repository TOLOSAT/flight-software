# Principal Makefile

##############################################
################## PROJECT ###################
##############################################

PROJ_NAME 		= boot-software
BOARD 			?= ART_PI
VERSION 		?= debug
FS_MODE			?= sdmmc

##############################################
################### MAKE #####################
##############################################

.PHONY = all clean echoes

all : clean build upload

include gen/settings.mk
include gen/path.mk
include gen/build.mk
include gen/debug.mk

clean :
	rm -rf $(BUILD_DIR)
