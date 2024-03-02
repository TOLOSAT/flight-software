# Principal Makefile

##############################################
################## PROJECT ###################
##############################################

PROJ_NAME 		= flight-software
BOARD 			= ART_PI
VERSION 		= debug
LOAD_MEMORY 	= ram
CONSOLE_MODE 	= uart
FS_MODE			= sdmmc
TEST 			=

##############################################
################### MAKE #####################
##############################################

.PHONY = all clean echoes

all : clean pre-build build upload

include gen/settings.mk
include gen/path.mk
include gen/build.mk
include gen/tests.mk
include gen/debug.mk
include gen/code_verification.mk

clean :
	rm -rf $(BUILD_DIR)

echoes :
	@echo $(PROJ_NAME)
