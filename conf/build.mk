# Makefile pour compiler

include conf/build_core.mk
include conf/build_os.mk
include conf/build_hal.mk
include conf/build_bsp.mk

##############################################
#################### BUILD ###################
##############################################

LINKER_SCRIPT = $(LINKER_SCRIPTS_DIR)/stm32f4_ls.ld
TARGET = $(TARGET_DIR)/$(PROJ_NAME).elf
TARGET_MAP = $(TARGET:.elf=.map)

.PHONY += build

build : $(TARGET)

# Links objects into target
$(TARGET) : core os bsp libhal libhal-tolosat
	mkdir -p $(@D)
	$(CC) ${CORE_OBJS} $(OS_OBJS) $(OS_CMSIS_OBJS) ${BSP_OBJS} -L$(BUILD_LIBS_DIR) -lhal-tolosat -lhal $(GENERIC_LDFLAGS) -o $@
	@echo "*****************************"
	@echo "***   Target Build Done   ***"
	@echo "*****************************"