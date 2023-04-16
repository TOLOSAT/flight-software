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

$(TARGET) : $(CMSIS_OBJS) $(HAL_OBJS) ${MAIN_OBJS} 
	mkdir -p $(@D)
	$(CC) $^ $(LDFLAGS) -o $@
	@echo "*****************************"
	@echo "***   Target Build Done   ***"
	@echo "*****************************"