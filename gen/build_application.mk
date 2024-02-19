# Application Building Makefile

##############################################
################ APPLICATION #################
##############################################

# Application Flags
APPLICATION_CFLAGS    = $(GENERIC_CFLAGS)
APPLICATION_INCFLAGS  = -I$(APPLICATION_INCDIR)
APPLICATION_INCFLAGS += -I$(CORE_INCDIR)
APPLICATION_INCFLAGS += -I$(PRE_BUILD_DIR)
APPLICATION_INCFLAGS += -I$(LIBPUS_INCDIR)
APPLICATION_INCFLAGS += -I$(LIBTIME_INCDIR)
APPLICATION_INCFLAGS += -I$(TOLOSAT_FS_INCDIR)
APPLICATION_INCFLAGS += -I$(IRIDIUM_DRIVER_INCDIR)
APPLICATION_INCFLAGS += -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
APPLICATION_INCFLAGS += -I$(GENERIC_HAL_INCDIR) -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
APPLICATION_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
APPLICATION_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
APPLICATION_INCFLAGS += -I$(BSP_INCDIR)

# Application Files
APPLICATION_SRCS = $(wildcard $(APPLICATION_SRCDIR)/*.c $(APPLICATION_SRCDIR)/*/*.c $(PRE_BUILD_DIR)/conf/*.c)
APPLICATION_OBJS = $(patsubst $(APPLICATION_SRCDIR)/%.c,$(BUILD_APPLICATION_DIR)/%-$(VERSION).o,$(patsubst $(PRE_BUILD_DIR)/conf/%.c,$(BUILD_APPLICATION_DIR)/%-$(VERSION).o,$(APPLICATION_SRCS)))
APPLICATION_LIB	 = $(BUILD_LIBS_DIR)/libapplication-$(VERSION).a

# Application compilation
$(BUILD_APPLICATION_DIR)/%-$(VERSION).o : $(APPLICATION_SRCDIR)/%.c 
	mkdir -p $(@D)
	$(CC) $(APPLICATION_CFLAGS) $(APPLICATION_INCFLAGS) $(VERSION_FLAGS) $^ -o $@

$(BUILD_APPLICATION_DIR)/%-$(VERSION).o  : $(PRE_BUILD_DIR)/conf/%.c
	mkdir -p $(@D)
	$(CC) $(APPLICATION_CFLAGS) $(APPLICATION_INCFLAGS) $(VERSION_FLAGS) $^ -o $@

# Application Library
$(APPLICATION_LIB) : $(APPLICATION_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^

# Application Recipe
application : $(APPLICATION_LIB)
	@echo "**************************************"
	@echo "*****   Application Build Done   *****"
	@echo "**************************************"
	@echo