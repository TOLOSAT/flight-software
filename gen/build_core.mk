# Core Building Makefile

##############################################
################### CORE #####################
##############################################

# Main Flags
CORE_CFLAGS    = $(PROJECT_CFLAGS)
CORE_INCFLAGS  = -I$(CORE_INCDIR)
CORE_INCFLAGS += -I$(APPLICATION_INCDIR)
CORE_INCFLAGS += -I$(PRE_BUILD_DIR)
CORE_INCFLAGS += -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
CORE_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
CORE_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
CORE_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE) 
CORE_INCFLAGS += -I$(BSP_INCDIR)

# System defines (those are use for system info const struct)
SYSTEM_DEFINES  = -DPROGRAM_NAME=\"$(PROJ_NAME)\"
SYSTEM_DEFINES += -DVERSION=\"$(VERSION)\"
SYSTEM_DEFINES += -DBUILD_TYPE=\"$(BUILD_TYPE)\"
SYSTEM_DEFINES += -DBOARD=\"$(BOARD)\"

# Main Files
CORE_SRCS = $(wildcard $(CORE_SRCDIR)/*.c $(CORE_SRCDIR)/*/*.c $(CORE_SRCDIR)/drv/$(CHIP_VENDOR)-wrapper/*.c)
ifneq ($(FS_MODE), NONE)
CORE_SRCS += $(CORE_SRCDIR)/drv/$(CHIP_VENDOR)-wrapper/disk/diskdrv_$(shell echo $(FS_MODE) | tr '[:upper:]' '[:lower:]').c
endif
CORE_OBJS = $(subst $(CORE_SRCDIR)/,$(BUILD_CORE_DIR)/,$(CORE_SRCS:.c=-$(BUILD_TYPE).o))
CORE_LIB  = $(BUILD_LIBS_DIR)/libcore-$(BUILD_TYPE).a

# Main compilation
$(BUILD_CORE_DIR)/%-$(BUILD_TYPE).o : $(CORE_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(CORE_CFLAGS) $(CORE_INCFLAGS) $(VERSION_FLAGS) $^ -o $@

# Special recipe for sys_info file
$(BUILD_CORE_DIR)/utils/sys_info-$(BUILD_TYPE).o : $(CORE_SRCDIR)/utils/sys_info.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(CORE_CFLAGS) $(SYSTEM_DEFINES) $(CORE_INCFLAGS) $(VERSION_FLAGS) $^ -o $@

# Core Library
$(CORE_LIB) : $(CORE_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Core Recipe
core-start :
	@echo "**************************************"
	@echo "********   CORE Start Build   ********"
	@echo "**************************************"

core-end :
	@echo "**************************************"
	@echo "*********   CORE Build Done   ********"
	@echo "**************************************"
	@echo

core: core-start $(CORE_LIB) core-end