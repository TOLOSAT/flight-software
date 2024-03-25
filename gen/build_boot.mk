# Makefile for boot

##############################################
################## BOOT CORE #################
##############################################

# Boot Flags
BOOT_CFLAGS    = $(PROJECT_CFLAGS)
BOOT_INCFLAGS  = -I$(BOOT_INCDIR)
BOOT_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
BOOT_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
BOOT_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
BOOT_INCFLAGS += -I$(BSP_INCDIR)

# Boot Files
BOOT_SRCS = $(wildcard $(BOOT_SRCDIR)/*.c)
BOOT_OBJS = $(subst $(BOOT_SRCDIR)/,$(BUILD_BOOT_DIR)/,$(BOOT_SRCS:.c=-$(VERSION).o))
BOOT_LIB  = $(BUILD_LIBS_DIR)/libbootcore-$(VERSION).a

# Boot compilation
$(BUILD_BOOT_DIR)/%-$(VERSION).o : $(BOOT_SRCDIR)/%.c 
	mkdir -p $(@D)
	$(CC) $(BOOT_CFLAGS) $(BOOT_INCFLAGS) $(VERSION_FLAGS) $^ -o $@

# Boot Core Library
$(BOOT_LIB) : $(BOOT_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^

# Boot Core Recipe
bootcore : $(BOOT_LIB)
	@echo "**************************************"
	@echo "******   Boot Core Build Done   ******"
	@echo "**************************************"
	@echo