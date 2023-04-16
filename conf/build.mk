# Makefile pour compiler

include conf/build_core.mk
include conf/build_hal.mk
include conf/build_cmsis.mk

##############################################
#################### BUILD ###################
##############################################

TARGET = $(TARGET_DIR)/$(PROJ_NAME).elf
TARGET_MAP = $(TARGET:.elf=.map)

.PHONY += build

build : libhal libcmsis $(TARGET)

$(TARGET) : core libcmsis libhal 
	mkdir -p $(@D)
	$(CC) ${CORE_OBJS} -L$(BUILD_LIBS_DIR) -lcmsis -lhal $(LDFLAGS) -o $@
	@echo "*****************************"
	@echo "***   Target Build Done   ***"
	@echo "*****************************"