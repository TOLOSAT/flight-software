# Makefile pour compiler

include conf/build_core.mk
include conf/build_os.mk
include conf/build_hal.mk
include conf/build_bsp.mk

##############################################
#################### BUILD ###################
##############################################

# Linkers Scripts
LINKER_SCRIPT  = $(LINKER_SCRIPTS_DIR)/stm32f4_ls.ld

# Targets definitions
TARGET_DBG     = $(TARGET_DIR)/$(PROJ_NAME)-debug.elf
TARGET_DBG_MAP = $(TARGET_DBG:.elf=.map)

TARGET_RLS     = $(TARGET_DIR)/$(PROJ_NAME)-release.elf
TARGET_RLS_MAP = $(TARGET_RLS:.elf=.map)

# Target definition according to version
ifeq ($(VERSION), DEBUG)
	TARGET = $(TARGET_DBG)
else ifeq ($(VERSION), RELEASE)
	TARGET = $(TARGET_RLS)
else
	print := $(error Please select DEBUG or RELEASE)
endif

.PHONY += build

build : $(TARGET)

# Debug Target Linking Stage
$(TARGET_DBG) : core-dbg os-dbg bsp-dbg libhal-dbg libhal-tolosat-dbg
	mkdir -p $(@D)
	$(CC) ${CORE_DBG_OBJS} $(OS_DBG_OBJS) ${BSP_DBG_OBJS} -L$(BUILD_LIBS_DIR) -lhal-tolosat-debug -lhal-debug $(GENERIC_LDFLAGS) -Wl,-Map=$(TARGET_DBG_MAP) -o $@
	@echo "*****************************"
	@echo "***   Target Build Done   ***"
	@echo "*****************************"

# Realease Target Linking Stage
$(TARGET_RLS) : core-rls os-rls bsp-rls libhal-rls libhal-tolosat-rls
	mkdir -p $(@D)
	$(CC) ${CORE_RLS_OBJS} $(OS_RLS_OBJS) ${BSP_RLS_OBJS} -L$(BUILD_LIBS_DIR) -lhal-tolosat-release -lhal-release $(GENERIC_LDFLAGS) -Wl,-Map=$(TARGET_DBG_MAP) -o $@
	@echo "*****************************"
	@echo "***   Target Build Done   ***"
	@echo "*****************************"