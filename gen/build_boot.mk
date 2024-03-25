# Makefile for boot

##############################################
################## BOOT CORE #################
##############################################

# Boot Flags
BOOT_CFLAGS = $(subst -DLOAD_RAM,-DLOAD_FLASH,$(PROJECT_CFLAGS))
BOOT_INCFLAGS  = -I$(BOOT_INCDIR)
BOOT_INCFLAGS += -I$(TOLOSAT_FS_INCDIR)
BOOT_INCFLAGS += -I$(GENERIC_HAL_INCDIR) -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
BOOT_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
BOOT_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
BOOT_INCFLAGS += -I$(BSP_INCDIR)

# Boot Files
BOOT_SRCS = $(filter-out %tolosat_fs.c, $(wildcard $(BOOT_SRCDIR)/*.c $(BSP_SRCDIR)/*.c $(BSP_SRCDIR)/*.s $(TOLOSAT_FS_SRCDIR)/*.c))
BOOT_OBJS = $(patsubst $(BOOT_SRCDIR)/%.c,$(BUILD_BOOT_DIR)/%-$(VERSION).o,$(filter $(BOOT_SRCDIR)/%.c,$(BOOT_SRCS))) \
			$(patsubst $(TOLOSAT_FS_SRCDIR)/%.c,$(BUILD_BOOT_DIR)/%-$(VERSION).o,$(filter $(TOLOSAT_FS_SRCDIR)/%.c,$(BOOT_SRCS))) \
			$(patsubst $(BSP_SRCDIR)/%.c,$(BUILD_BOOT_DIR)/%-$(VERSION).o,$(filter $(BSP_SRCDIR)/%.c,$(BOOT_SRCS)))
BOOT_LIB  = $(BUILD_LIBS_DIR)/libbootsw-$(VERSION).a

# Boot compilation
$(BUILD_BOOT_DIR)/%-$(VERSION).o : $(BOOT_SRCDIR)/%.c 
	mkdir -p $(@D)
	$(CC) $(BOOT_CFLAGS) $(BOOT_INCFLAGS) $(VERSION_FLAGS) $^ -o $@

$(BUILD_BOOT_DIR)/%-$(VERSION).o : $(TOLOSAT_FS_SRCDIR)/%.c 
	mkdir -p $(@D)
	$(CC) $(BOOT_CFLAGS) $(BOOT_INCFLAGS) $(VERSION_FLAGS) $^ -o $@

$(BUILD_BOOT_DIR)/%-$(VERSION).o : $(BSP_SRCDIR)/%.c 
	mkdir -p $(@D)
	$(CC) $(BOOT_CFLAGS) $(BOOT_INCFLAGS) $(VERSION_FLAGS) $^ -o $@

# Boot Core Library
$(BOOT_LIB) : $(BOOT_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^

# Boot Core Recipe
bootsw : $(BOOT_LIB)
	@echo "**************************************"
	@echo "******   Boot Core Build Done   ******"
	@echo "**************************************"
	@echo