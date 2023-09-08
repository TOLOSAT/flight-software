# MIDDLEWARES Building Makefile

##############################################
################## LIBPUS ####################
##############################################

# LIBPUS Flags
LIBPUS_CFLAGS    = $(GENERIC_CFLAGS)
LIBPUS_INCFLAGS  = -I$(LIBPUS_INCDIR)
LIBPUS_INCFLAGS += -I$(HAL_TOLOSAT_INCDIR)
LIBPUS_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
LIBPUS_INCFLAGS += -I$(TOLOSAT_FS_INCDIR)
LIBPUS_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
LIBPUS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
LIBPUS_INCFLAGS += -I$(CMSIS_RTOS2_INCDIR)

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

libpus : $(LIBPUS_LIB)
	@echo $(LIBPUS_SRCDIR)
	@echo "*********************************"
	@echo "*****   LibPUS Build Done   *****"
	@echo "*********************************"
	@echo

##############################################
################ TOLOSAT FS ##################
##############################################

# TOLOSAT_FS Flags
TOLOSAT_FS_CFLAGS    = $(GENERIC_CFLAGS)
TOLOSAT_FS_INCFLAGS  = -I$(TOLOSAT_FS_INCDIR)
TOLOSAT_FS_INCFLAGS += -I$(CORE_INCDIR)
TOLOSAT_FS_INCFLAGS += -I$(HAL_TOLOSAT_INCDIR)
TOLOSAT_FS_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
TOLOSAT_FS_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
TOLOSAT_FS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
TOLOSAT_FS_INCFLAGS += -I$(CMSIS_RTOS2_INCDIR)
TOLOSAT_FS_INCFLAGS += -I$(BSP_INCDIR)

# TOLOSAT_FS Files
TOLOSAT_FS_SRCS = $(wildcard $(TOLOSAT_FS_SRCDIR)/*.c $(TOLOSAT_FS_SRCDIR)/*/*.c)
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

libtolosat-fs : $(TOLOSAT_FS_LIB)
	@echo $(TOLOSAT_FS_SRCDIR)
	@echo "*************************************"
	@echo "*****   TOLOSAT FS Build Done   *****"
	@echo "*************************************"
	@echo