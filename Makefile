# Makefile de génération du flight software

##############################################
################## PROJECT ###################
##############################################

PROJ_NAME = flight-software
BOARD = NUCLEO-F411RE
VERSION = debug
TEST =

##############################################
################### MAKE #####################
##############################################

.PHONY = all doxy clean echoes

all : clean build flash

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
