# Makefile pour compiler

include conf/build_core.mk
include conf/build_os.mk
include conf/build_hal.mk
include conf/build_bsp.mk

##############################################
#################### BUILD ###################
##############################################

# Targets definitions
TARGET_DBG     = $(TARGET_DIR)/$(PROJ_NAME)-$(VERSION).elf
TARGET_RLS     = $(TARGET_DIR)/$(PROJ_NAME)-$(VERSION).elf

# Target definition according to version
ifeq ($(VERSION), debug)
	TARGET = $(TARGET_DBG)
	VERSION_FLAGS = $(DEBUG_FLAGS)
else ifeq ($(VERSION), release)
	TARGET = $(TARGET_RLS)
	VERSION_FLAGS = $(RELEASE_FLAGS)
else
	print := $(error Please select DEBUG or RELEASE)
endif

.PHONY += build

build : $(TARGET)

# Target Linking Stage
$(TARGET) : core os bsp libhal libhal-tolosat
	mkdir -p $(@D)
	$(CC) ${CORE_OBJS} $(OS_OBJS) ${BSP_OBJS} -L$(BUILD_LIBS_DIR) -lhal-tolosat-$(VERSION) -lhal-$(VERSION) $(GENERIC_LDFLAGS) -o $@
	$(SIZE) $(TARGET) > $(TARGET:.elf=.size)
	@echo "*****************************"
	@echo "***   Target Build Done   ***"
	@echo "*****************************"