# Principal Makefile

##############################################
################## PROJECT ###################
##############################################

PROJ_NAME 		= boot-software
BUILD_TYPE 		?= debug
BOARD 			?= ART_PI
FS_MODE			?= SDMMC

##############################################
################### MAKE #####################
##############################################

.PHONY = all clean echoes

all : build

include gen/settings.mk
include gen/path.mk
include gen/build.mk
include gen/debug.mk
include gen/code_verification.mk

clean :
	@rm -rf $(BUILD_DIR)
