# Makefile de génération du flight software

##############################################
################## PROJECT ###################
##############################################

PROJ_NAME=flight-software
BOARD = NUCLEO-F411RE

##############################################
################### MAKE #####################
##############################################

.PHONY = all clean clean-all echoes

all : clean-all build flash

build : $(TARGET)

clean :
	rm -rf $(TARGET) $(MAIN_OBJDIR)

clean-all : 
	rm -rf $(BUILD_DIR)

echoes :
	@echo $(.PHONY)

include conf/path.mk
include conf/settings.mk
include conf/build.mk
include conf/debug.mk