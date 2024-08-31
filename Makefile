# Principal Makefile

##############################################
################ CONFIGURATION ###############
##############################################

# Software Version
VERSION			= v1.6.8

# Project default configuration, do not modify
PROJ_NAME		?= flight-software
BUILD_TYPE		?= debug
BOARD			?= ART_PI
LOAD_MEMORY		?= RAM
CONSOLE_MODE	?= FILE
FS_MODE			?= SD
TEST			?=

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
	make -C $(BOOT_DIR) BOARD=$(BOARD) BUILD_TYPE=$(BUILD_TYPE) FS_MODE=$(FS_MODE)

config :
	$(GEN_DIR)/menuconfig.sh

clean :
	rm -rf $(BUILD_DIR)

echoes :
	@echo $(PROJ_NAME)
