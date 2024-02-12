# Principal Makefile

##############################################
################## PROJECT ###################
##############################################

PROJ_NAME = flight-software
BOARD = NUCLEO_H745ZI
LOAD_MEMORY = flash
VERSION = debug
TEST =

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
