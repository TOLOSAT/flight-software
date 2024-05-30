# MIDDLEWARES Building Makefile

##############################################
################## LIBPUS ####################
##############################################

# LIBPUS Flags
LIBPUS_CFLAGS    = $(PROJECT_CFLAGS) -DLPUS_EXTERNAL_TIME_MGMT
LIBPUS_INCFLAGS  = -I$(LIBPUS_INCDIR)
LIBPUS_INCFLAGS += -I$(APPLICATION_INCDIR)
LIBPUS_INCFLAGS += -I$(PRE_BUILD_DIR)
LIBPUS_INCFLAGS += -I$(LIBTIME_INCDIR)
LIBPUS_INCFLAGS += -I$(LIBFS_INCDIR)
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
################### LIBTIME ##################
##############################################

# LIBTIME Flags
LIBTIME_CFLAGS    = $(PROJECT_CFLAGS)
LIBTIME_INCFLAGS  = -I$(LIBTIME_INCDIR)
LIBTIME_INCFLAGS += -I$(GENERIC_HAL_INCDIR) -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
LIBTIME_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
LIBTIME_INCFLAGS += -I$(BSP_INCDIR)

# LIBTIME Files
LIBTIME_SRCS = $(wildcard $(LIBTIME_SRCDIR)/*.c)
LIBTIME_OBJS = $(subst $(LIBTIME_SRCDIR)/,$(LIBTIME_OBJDIR)/,$(LIBTIME_SRCS:.c=-$(BUILD_TYPE).o))
LIBTIME_LIB  = $(BUILD_LIBS_DIR)/libtime-$(BUILD_TYPE).a

# LIBTIME compilation
$(LIBTIME_OBJDIR)/%-$(BUILD_TYPE).o : $(LIBTIME_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(LIBTIME_CFLAGS) $(LIBTIME_INCFLAGS) $(VERSION_FLAGS) $^ -o $@ 

# LIBTIME Library
$(LIBTIME_LIB) : $(LIBTIME_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^

# LIBTIME Recipe
time : $(LIBTIME_LIB)
	@echo $(LIBTIME_SRCDIR)
	@echo "*********************************"
	@echo "*****   LIBTIME Build Done   *****"
	@echo "*********************************"
	@echo

##############################################
#################### LIBFS ###################
##############################################

# LIBFS Flags
LIBFS_CFLAGS    = $(PROJECT_CFLAGS)
LIBFS_INCFLAGS  = -I$(LIBFS_INCDIR)
LIBFS_INCFLAGS += -I$(CORE_INCDIR)
LIBFS_INCFLAGS += -I$(APPLICATION_INCDIR)
LIBFS_INCFLAGS += -I$(PRE_BUILD_DIR)
LIBFS_INCFLAGS += -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
LIBFS_INCFLAGS += -I$(GENERIC_HAL_INCDIR) -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
LIBFS_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
LIBFS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
LIBFS_INCFLAGS += -I$(BSP_INCDIR)

# LIBFS Files
LIBFS_SRCS = $(wildcard $(LIBFS_SRCDIR)/*.c)
LIBFS_OBJS = $(subst $(LIBFS_SRCDIR)/,$(LIBFS_OBJDIR)/,$(LIBFS_SRCS:.c=-$(BUILD_TYPE).o))
LIBFS_LIB  = $(BUILD_LIBS_DIR)/libfs-$(BUILD_TYPE).a

# LIBFS compilation
$(LIBFS_OBJDIR)/%-$(BUILD_TYPE).o : $(LIBFS_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(LIBFS_CFLAGS) $(LIBFS_INCFLAGS) $(VERSION_FLAGS) $^ -o $@ 

# LIBFS Library
$(LIBFS_LIB) : $(LIBFS_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^

# LIBFS Recipe
fs : $(LIBFS_LIB)
	@echo $(LIBFS_SRCDIR)
	@echo "*************************************"
	@echo "********   LIBFS Build Done   *******"
	@echo "*************************************"
	@echo

##############################################
############### Iridium Driver ###############
##############################################

# IRIDIUM_DRV Flags
IRIDIUM_DRV_CFLAGS    = $(PROJECT_CFLAGS)
IRIDIUM_DRV_INCFLAGS  = -I$(IRIDIUM_DRV_INCDIR)
IRIDIUM_DRV_INCFLAGS += -I$(GENERIC_HAL_INCDIR) -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
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

##############################################
################ GENERIC HAL #################
##############################################

# GENERIC HAL Flags
GENERIC_HAL_CFLAGS    = $(PROJECT_CFLAGS)
GENERIC_HAL_INCFLAGS  = -I$(GENERIC_HAL_INCDIR)
GENERIC_HAL_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
GENERIC_HAL_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
GENERIC_HAL_INCFLAGS += -I$(BSP_INCDIR)

# GENERIC HAL Files
GENERIC_HAL_SRCS = $(wildcard $(GENERIC_HAL_SRCDIR)/*.c)
GENERIC_HAL_OBJS = $(subst $(GENERIC_HAL_SRCDIR)/,$(GENERIC_HAL_OBJDIR)/,$(GENERIC_HAL_SRCS:.c=-$(BUILD_TYPE).o))
GENERIC_HAL_LIB  = $(BUILD_LIBS_DIR)/libgeneric-hal-$(BUILD_TYPE).a

# GENERIC HAL compilation
$(GENERIC_HAL_OBJDIR)/%-$(BUILD_TYPE).o : $(GENERIC_HAL_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(GENERIC_HAL_CFLAGS) $(GENERIC_HAL_INCFLAGS) $(VERSION_FLAGS) $^ -o $@ 

# GENERIC HAL Library
$(GENERIC_HAL_LIB) : $(GENERIC_HAL_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^

# Generic HAL Recipe
generic-hal : $(GENERIC_HAL_LIB)
	@echo "********************************"
	@echo "**   GENERIC HAL Build Done   **"
	@echo "********************************"
	@echo