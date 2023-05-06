# Makefile de génération du flight software

##############################################
################## PROJECT ###################
##############################################

PROJ_NAME = flight-software
BOARD = NUCLEO-F411RE
VERSION = debug

##############################################
################### MAKE #####################
##############################################

.PHONY = all clean echoes

all : clean verif build flash

include conf/settings.mk
include conf/path.mk
include conf/build.mk
include conf/debug.mk
include conf/code_verification.mk

clean :
	rm -rf $(BUILD_DIR)

echoes :
	@echo $(PROJ_NAME)
