# Principal Makefile

##############################################
################ CONFIGURATION ###############
##############################################

# Software Version
VERSION			= v1.6.8

##############################################
################### MAKE #####################
##############################################

.PHONY = all boot clean config echoes

all : build

include gen/build.mk
include gen/debug.mk
include gen/configuration.mk
include gen/verification.mk

boot :
	make -C $(BOOT_DIR) BOARD=$(BOARD) BUILD_TYPE=$(BUILD_TYPE) FS_MODE=$(FS_MODE)

clean :
	@rm -rf $(BUILD_DIR)

echoes :
	@echo $(PROJ_NAME)
