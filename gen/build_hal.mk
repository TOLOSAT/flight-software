# HAL Building Makefile

##############################################
#################### HAL #####################
##############################################

# HAL Flags
HAL_CFLAGS    = $(GENERIC_CFLAGS) -Wno-unused-variable -Wno-unused-parameter
HAL_INCFLAGS  = -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
HAL_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)

# HAL Files
include $(HAL_SRCS_LIST)
HAL_OBJS  = $(subst $(HAL_SRCDIR)/,$(HAL_OBJDIR)/,$(HAL_SRCS:.c=-$(VERSION).o))
HAL_LIB   = $(BUILD_LIBS_DIR)/libhal-$(VERSION).a

# HAL compilation
$(HAL_OBJDIR)/%-$(VERSION).o : $(HAL_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(HAL_CFLAGS) $(HAL_INCFLAGS) $(VERSION_FLAGS) $^ -o $@ 

# HAL Library
$(HAL_LIB) : $(HAL_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^

libhal : $(HAL_LIB)
	@echo "******************************"
	@echo "*****   HAL Build Done   *****"
	@echo "******************************"
	@echo

##############################################
################ FATS LIBRARY ################
##############################################

# FATFS Flags
FATFS_CFLAGS    = $(GENERIC_CFLAGS) -Wno-unused-variable -Wno-unused-parameter -Wno-stringop-overflow
FATFS_INCFLAGS  = -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
FATFS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)

# FATFS Files
FATFS_SRCS  = $(wildcard $(FATFS_SRCDIR)/*.c)
FATFS_OBJS  = $(subst $(FATFS_SRCDIR)/,$(FATFS_OBJDIR)/,$(FATFS_SRCS:.c=-$(VERSION).o))
FATFS_LIB   = $(BUILD_LIBS_DIR)/libfatfs-$(VERSION).a

# FATFS compilation
$(FATFS_OBJDIR)/%-$(VERSION).o : $(FATFS_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(FATFS_CFLAGS) $(FATFS_INCFLAGS) $(VERSION_FLAGS) $^ -o $@ 

# FATFS Library
$(FATFS_LIB) : $(FATFS_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^

libfatfs : $(FATFS_LIB)
	@echo "********************************"
	@echo "*****   FATFS Build Done   *****"
	@echo "********************************"
	@echo

##############################################
################ HAL TOLOSAT #################
##############################################

# HAL TOLOSAT Flags
HAL_TOLOSAT_CFLAGS    = $(GENERIC_CFLAGS)
HAL_TOLOSAT_INCFLAGS  = -I$(HAL_TOLOSAT_INCDIR)
HAL_TOLOSAT_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
HAL_TOLOSAT_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
HAL_TOLOSAT_INCFLAGS += -I$(BSP_INCDIR)

# HAL TOLOSAT Files
HAL_TOLOSAT_SRCS = $(wildcard $(HAL_TOLOSAT_SRCDIR)/*.c)
HAL_TOLOSAT_OBJS = $(subst $(HAL_TOLOSAT_SRCDIR)/,$(HAL_TOLOSAT_OBJDIR)/,$(HAL_TOLOSAT_SRCS:.c=-$(VERSION).o))
HAL_TOLOSAT_LIB  = $(BUILD_LIBS_DIR)/libhal-tolosat-$(VERSION).a

# HAL TOLOSAT compilation
$(HAL_TOLOSAT_OBJDIR)/%-$(VERSION).o : $(HAL_TOLOSAT_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(HAL_TOLOSAT_CFLAGS) $(HAL_TOLOSAT_INCFLAGS) $(VERSION_FLAGS) $^ -o $@ 

# HAL TOLOSAT Library
$(HAL_TOLOSAT_LIB) : $(HAL_TOLOSAT_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^

libhal-tolosat : $(HAL_TOLOSAT_LIB)
	@echo "********************************"
	@echo "**   HAL TOLOSAT Build Done   **"
	@echo "********************************"
	@echo