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
LIBPUS_INCFLAGS += -I$(TOLOSAT_FS_INCDIR)
LIBPUS_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
LIBPUS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
LIBPUS_INCFLAGS += -I$(BSP_INCDIR)

# LIBPUS Files
LIBPUS_SRCS = $(wildcard $(LIBPUS_SRCDIR)/*.c $(LIBPUS_SRCDIR)/*/*.c)
LIBPUS_OBJS = $(subst $(LIBPUS_SRCDIR)/,$(LIBPUS_OBJDIR)/,$(LIBPUS_SRCS:.c=-$(VERSION).o))
LIBPUS_LIB  = $(BUILD_LIBS_DIR)/libpus-$(VERSION).a

# LIBPUS compilation
$(LIBPUS_OBJDIR)/%-$(VERSION).o : $(LIBPUS_SRCDIR)/%.c
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
LIBTIME_OBJS = $(subst $(LIBTIME_SRCDIR)/,$(LIBTIME_OBJDIR)/,$(LIBTIME_SRCS:.c=-$(VERSION).o))
LIBTIME_LIB  = $(BUILD_LIBS_DIR)/libtime-$(VERSION).a

# LIBTIME compilation
$(LIBTIME_OBJDIR)/%-$(VERSION).o : $(LIBTIME_SRCDIR)/%.c
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
################ TOLOSAT FS ##################
##############################################

# TOLOSAT_FS Flags
TOLOSAT_FS_CFLAGS    = $(PROJECT_CFLAGS)
TOLOSAT_FS_INCFLAGS  = -I$(TOLOSAT_FS_INCDIR)
TOLOSAT_FS_INCFLAGS += -I$(CORE_INCDIR)
TOLOSAT_FS_INCFLAGS += -I$(APPLICATION_INCDIR)
TOLOSAT_FS_INCFLAGS += -I$(PRE_BUILD_DIR)
TOLOSAT_FS_INCFLAGS += -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
TOLOSAT_FS_INCFLAGS += -I$(GENERIC_HAL_INCDIR) -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
TOLOSAT_FS_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
TOLOSAT_FS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
TOLOSAT_FS_INCFLAGS += -I$(BSP_INCDIR)

# TOLOSAT_FS Files
TOLOSAT_FS_SRCS = $(wildcard $(TOLOSAT_FS_SRCDIR)/*.c)
TOLOSAT_FS_OBJS = $(subst $(TOLOSAT_FS_SRCDIR)/,$(TOLOSAT_FS_OBJDIR)/,$(TOLOSAT_FS_SRCS:.c=-$(VERSION).o))
TOLOSAT_FS_LIB  = $(BUILD_LIBS_DIR)/libtolosat-fs-$(VERSION).a

# TOLOSAT_FS compilation
$(TOLOSAT_FS_OBJDIR)/%-$(VERSION).o : $(TOLOSAT_FS_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(TOLOSAT_FS_CFLAGS) $(TOLOSAT_FS_INCFLAGS) $(VERSION_FLAGS) $^ -o $@ 

# TOLOSAT_FS Library
$(TOLOSAT_FS_LIB) : $(TOLOSAT_FS_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^

# TOLOSAT FS Recipe
tolosat-fs : $(TOLOSAT_FS_LIB)
	@echo $(TOLOSAT_FS_SRCDIR)
	@echo "*************************************"
	@echo "*****   TOLOSAT FS Build Done   *****"
	@echo "*************************************"
	@echo

##############################################
############### Iridium Driver ###############
##############################################

# IRIDIUM_DRIVER Flags
IRIDIUM_DRIVER_CFLAGS    = $(PROJECT_CFLAGS)
IRIDIUM_DRIVER_INCFLAGS  = -I$(IRIDIUM_DRIVER_INCDIR)
IRIDIUM_DRIVER_INCFLAGS += -I$(GENERIC_HAL_INCDIR) -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
IRIDIUM_DRIVER_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE) 
IRIDIUM_DRIVER_INCFLAGS += -I$(BSP_INCDIR)

# IRIDIUM_DRIVER Files
IRIDIUM_DRIVER_SRCS = $(wildcard $(IRIDIUM_DRIVER_SRCDIR)/*.c)
IRIDIUM_DRIVER_OBJS = $(subst $(IRIDIUM_DRIVER_SRCDIR)/,$(IRIDIUM_DRIVER_OBJDIR)/,$(IRIDIUM_DRIVER_SRCS:.c=-$(VERSION).o))
IRIDIUM_DRIVER_LIB  = $(BUILD_LIBS_DIR)/libiridiumdrv-$(VERSION).a

# IRIDIUM_DRIVER compilation
$(IRIDIUM_DRIVER_OBJDIR)/%-$(VERSION).o : $(IRIDIUM_DRIVER_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(IRIDIUM_DRIVER_CFLAGS) $(IRIDIUM_DRIVER_INCFLAGS) $(VERSION_FLAGS) $^ -o $@ 

# IRIDIUM_DRIVER Library
$(IRIDIUM_DRIVER_LIB) : $(IRIDIUM_DRIVER_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^

# Iridium Driver Recipe
iridiumdrv : $(IRIDIUM_DRIVER_LIB)
	@echo $(IRIDIUM_DRIVER_SRCDIR)
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
GENERIC_HAL_OBJS = $(subst $(GENERIC_HAL_SRCDIR)/,$(GENERIC_HAL_OBJDIR)/,$(GENERIC_HAL_SRCS:.c=-$(VERSION).o))
GENERIC_HAL_LIB  = $(BUILD_LIBS_DIR)/libgeneric-hal-$(VERSION).a

# GENERIC HAL compilation
$(GENERIC_HAL_OBJDIR)/%-$(VERSION).o : $(GENERIC_HAL_SRCDIR)/%.c
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