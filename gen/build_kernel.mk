# Kernel Building Makefile

ifndef BUILD_KERNEL_MK
BUILD_KERNEL_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk
include gen/cc_settings.mk

##############################################
################## KERNEL ####################
##############################################

# Disk driver selection
ifneq ($(CONFIG_FS_NONE), y)
ifeq ($(CONFIG_FS_SPISD), y)
DISKDRV = $(KERNEL_DISKDRV_SRCDIR)/diskdrv_spisd.c
else ifeq ($(CONFIG_FS_SD), y)
DISKDRV = $(KERNEL_DISKDRV_SRCDIR)/diskdrv_sd.c
else ifeq ($(CONFIG_FS_RAM), y)
DISKDRV = $(KERNEL_DISKDRV_SRCDIR)/diskdrv_ram.c
endif
endif

# Kernel files
KERNEL_SRCS = $(wildcard $(KERNEL_SRCDIR)/*.c $(KERNEL_SRCDIR)/*/*.c $(KERNEL_DRV_SRCDIR)/*.c $(DISKDRV)) $(CONF_SRCS)
KERNEL_OBJS = $(patsubst $(KERNEL_SRCDIR)/%.c,$(KERNEL_OBJDIR)/%-$(BUILD_TYPE).o,$(patsubst $(PRE_BUILD_DIR)/conf/%.c,$(KERNEL_OBJDIR)/conf/%-$(BUILD_TYPE).o,$(KERNEL_SRCS)))
KERNEL_LIB  = $(LIBS_DIR)/libkernel-$(BUILD_TYPE).a

# System defines (those are use for system info const struct)
SYSTEM_DEFINES  = -DSYSTEM_NAME=\"TAPAS\"
SYSTEM_DEFINES += -DPROGRAM_NAME=\"$(PROJ_NAME)\"
SYSTEM_DEFINES += -DVERSION=\"$(VERSION)\"
SYSTEM_DEFINES += -DBUILD_TYPE=\"$(BUILD_TYPE)\"
SYSTEM_DEFINES += -DBOARD=\"$(BOARD)\"

# Kernel flags
KERNEL_CFLAGS    = $(PROJECT_CFLAGS)
KERNEL_INCFLAGS  = -I$(KERNEL_INCDIR)
KERNEL_INCFLAGS += -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
KERNEL_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
KERNEL_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
KERNEL_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
KERNEL_INCFLAGS += -I$(PRE_BUILD_DIR)
KERNEL_INCFLAGS += -I$(BSP_INCDIR)

# Include dependencies
-include $(KERNEL_OBJS:.o=.d)

# Kernel recipes
.PHONY += kernel kernel-start kernel-end kernel-clean
kernel: kernel-start $(KERNEL_LIB) kernel-end

# Build header
kernel-start :
	@echo "============================="
	@echo "===         KERNEL        ==="
	@echo "============================="
	@echo "Files to compile: $(words $(KERNEL_SRCS))"
	@echo "Compilation Flags:"
	@echo $(KERNEL_CFLAGS)
	@echo "Include Paths:"
	@echo $(KERNEL_INCFLAGS)
	@echo "Version Flags:"
	@echo $(VERSION_FLAGS)
	@echo "Start building:"

# Building recipes
$(KERNEL_OBJDIR)/%-$(BUILD_TYPE).o : $(KERNEL_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(KERNEL_CFLAGS) $(KERNEL_INCFLAGS) $(VERSION_FLAGS) $< -o $@

$(KERNEL_OBJDIR)/conf/%-$(BUILD_TYPE).o  : $(PRE_BUILD_DIR)/conf/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(KERNEL_CFLAGS) $(KERNEL_INCFLAGS) $(VERSION_FLAGS) $< -o $@

$(KERNEL_OBJDIR)/system/sysinfo-$(BUILD_TYPE).o : $(KERNEL_SRCDIR)/system/sysinfo.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(KERNEL_CFLAGS) $(SYSTEM_DEFINES) $(KERNEL_INCFLAGS) $(VERSION_FLAGS) $< -o $@

# Library generation
$(KERNEL_LIB) : $(KERNEL_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
kernel-end :
	@echo "Build done"
	@echo ""

# Clean recipe
kernel-clean :
	@echo "Cleaning KERNEL build directory ..."
	@rm -rf $(KERNEL_OBJDIR)
	@rm -rf $(KERNEL_LIB)
	@echo "Done"

endif # BUILD_KERNEL_MK #