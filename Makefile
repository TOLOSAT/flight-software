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

include conf/settings.mk
include conf/path.mk
include conf/build.mk
include conf/tests.mk
include conf/debug.mk
include conf/code_verification.mk

clean :
	rm -rf $(BUILD_DIR)

echoes :
	@echo $(PROJ_NAME)
