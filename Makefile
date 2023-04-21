# Makefile de génération du flight software

##############################################
################## PROJECT ###################
##############################################

PROJ_NAME=flight-software
BOARD = NUCLEO-F411RE

##############################################
################### MAKE #####################
##############################################

.PHONY = all clean echoes

all : clean build flash

include conf/settings.mk
include conf/path.mk
include conf/build.mk
include conf/debug.mk

clean :
	rm -rf $(BUILD_DIR)

echoes :
	@echo $(CMSIS_RTOS2_INCDIR)