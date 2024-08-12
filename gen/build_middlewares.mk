# MIDDLEWARES Building Makefile

##############################################
################## LIBPUS ####################
##############################################

# LIBPUS Flags
LIBPUS_CFLAGS    = $(PROJECT_CFLAGS) -DLPUS_EXTERNAL_TIME_MGMT
LIBPUS_INCFLAGS  = -I$(LIBPUS_INCDIR)
LIBPUS_INCFLAGS += -I$(PRE_BUILD_DIR)
LIBPUS_INCFLAGS += -I$(CORE_INCDIR)
LIBPUS_INCFLAGS += -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
LIBPUS_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
LIBPUS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
LIBPUS_INCFLAGS += -I$(BSP_INCDIR)

# LIBPUS Files
LIBPUS_SRCS = $(wildcard $(LIBPUS_SRCDIR)/*.c $(LIBPUS_SRCDIR)/*/*.c)
LIBPUS_OBJS = $(subst $(LIBPUS_SRCDIR)/,$(LIBPUS_OBJDIR)/,$(LIBPUS_SRCS:.c=-$(BUILD_TYPE).o))
LIBPUS_LIB  = $(BUILD_LIBS_DIR)/libpus-$(BUILD_TYPE).a

# LIBPUS compilation
$(LIBPUS_OBJDIR)/%-$(BUILD_TYPE).o : $(LIBPUS_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(LIBPUS_CFLAGS) $(LIBPUS_INCFLAGS) $(VERSION_FLAGS) $^ -o $@ 

# LIBPUS Library
$(LIBPUS_LIB) : $(LIBPUS_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^

# LIBPUS Recipe
pus : $(LIBPUS_LIB)
	@echo $(LIBPUS_SRCDIR)
	@echo "*********************************"
	@echo "*****   LIBPUS Build Done   *****"
	@echo "*********************************"
	@echo

##############################################
############### Iridium Driver ###############
##############################################

# IRIDIUM_DRV Flags
IRIDIUM_DRV_CFLAGS    = $(PROJECT_CFLAGS)
IRIDIUM_DRV_INCFLAGS  = -I$(IRIDIUM_DRV_INCDIR)
IRIDIUM_DRV_INCFLAGS += -I$(GENERIC_HAL_INCDIR) -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
IRIDIUM_DRV_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
IRIDIUM_DRV_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE) 
IRIDIUM_DRV_INCFLAGS += -I$(BSP_INCDIR)

# IRIDIUM_DRV Files
IRIDIUM_DRV_SRCS = $(wildcard $(IRIDIUM_DRV_SRCDIR)/*.c)
IRIDIUM_DRV_OBJS = $(subst $(IRIDIUM_DRV_SRCDIR)/,$(IRIDIUM_DRV_OBJDIR)/,$(IRIDIUM_DRV_SRCS:.c=-$(BUILD_TYPE).o))
IRIDIUM_DRV_LIB  = $(BUILD_LIBS_DIR)/libiridiumdrv-$(BUILD_TYPE).a

# IRIDIUM_DRV compilation
$(IRIDIUM_DRV_OBJDIR)/%-$(BUILD_TYPE).o : $(IRIDIUM_DRV_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(IRIDIUM_DRV_CFLAGS) $(IRIDIUM_DRV_INCFLAGS) $(VERSION_FLAGS) $^ -o $@ 

# IRIDIUM_DRV Library
$(IRIDIUM_DRV_LIB) : $(IRIDIUM_DRV_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^

# Iridium Driver Recipe
iridiumdrv : $(IRIDIUM_DRV_LIB)
	@echo $(IRIDIUM_DRV_SRCDIR)
	@echo "*****************************************"
	@echo "*****   Iridium Driver Build Done   *****"
	@echo "*****************************************"
	@echo
