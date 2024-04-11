# Principal Makefile

##############################################
################## PROJECT ###################
##############################################

PROJ_NAME 		= flight-software
LOAD_MEMORY 	= ram
BOARD 			= ART_PI
VERSION 		= debug
CONSOLE_MODE 	= file
FS_MODE			= sdmmc
TEST 			=

##############################################
################### MAKE #####################
##############################################

.PHONY = all boot clean config echoes

all : clean pre-build build upload

include gen/settings.mk
include gen/path.mk
include gen/build.mk
include gen/tests.mk
include gen/debug.mk
include gen/code_verification.mk

boot :
	make -C $(BOOT_DIR) BOARD=$(BOARD) VERSION=$(VERSION) FS_MODE=$(FS_MODE)

config :
	./gen/menuconfig.sh

clean :
	rm -rf $(BUILD_DIR)

echoes :
	@echo $(PROJ_NAME)
