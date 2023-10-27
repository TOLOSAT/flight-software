# Core Building Makefile

##############################################
################### CORE #####################
##############################################

# Main Flags
CORE_CFLAGS    = $(GENERIC_CFLAGS)
CORE_INCFLAGS  = -I$(CORE_INCDIR)
CORE_INCFLAGS += -I$(APPLICATION_INCDIR)
CORE_INCFLAGS += -I$(LIBPUS_INCDIR)
CORE_INCFLAGS += -I$(TOLOSAT_FS_INCDIR)
CORE_INCFLAGS += -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
CORE_INCFLAGS += -I$(HAL_TOLOSAT_INCDIR) -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
CORE_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
CORE_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE) 
CORE_INCFLAGS += -I$(BSP_INCDIR)

# Main Files
CORE_SRCS = $(wildcard $(CORE_SRCDIR)/*.c)
CORE_OBJS = $(subst $(CORE_SRCDIR)/,$(BUILD_CORE_DIR)/,$(CORE_SRCS:.c=-$(VERSION).o))

# Main compilation
$(BUILD_CORE_DIR)/%-$(VERSION).o : $(CORE_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CORE_CFLAGS) $(CORE_INCFLAGS) $(VERSION_FLAGS) $^ -o $@

core: $(CORE_OBJS)
	@echo "*******************************"
	@echo "*****   Core Build Done   *****"
	@echo "*******************************"
	@echo