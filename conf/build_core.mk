# Core Building Makefile

##############################################
################### MAIN #####################
##############################################

# Main Flags
MAIN_CFLAGS    = $(GENERIC_CFLAGS)
MAIN_INCFLAGS  = -I$(MAIN_INCDIR)
MAIN_INCFLAGS += -I$(APPLICATION_INCDIR)
MAIN_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE) -I$(CMSIS_RTOS2_INCDIR)
MAIN_INCFLAGS += -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
MAIN_INCFLAGS += -I$(HAL_TOLOSAT_INCDIR)
MAIN_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
MAIN_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
MAIN_INCFLAGS += -I$(BSP_INCDIR)

# Main Files
MAIN_SRCS = $(wildcard $(MAIN_SRCDIR)/*.c)
MAIN_OBJS = $(subst $(MAIN_SRCDIR)/,$(MAIN_OBJDIR)/,$(MAIN_SRCS:.c=-$(VERSION).o))

# Main compilation
$(MAIN_OBJDIR)/%-$(VERSION).o : $(MAIN_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(MAIN_CFLAGS) $(MAIN_INCFLAGS) $(VERSION_FLAGS) $^ -o $@

##############################################
################ APPLICATION #################
##############################################

# Application Flags
APPLICATION_CFLAGS    = $(GENERIC_CFLAGS)
APPLICATION_INCFLAGS  = -I$(APPLICATION_INCDIR)
APPLICATION_INCFLAGS += -I$(MAIN_INCDIR)
APPLICATION_INCFLAGS += -I$(LIBPUS_INCDIR)
APPLICATION_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE) -I$(CMSIS_RTOS2_INCDIR)
APPLICATION_INCFLAGS += -I$(HAL_TOLOSAT_INCDIR)
APPLICATION_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
APPLICATION_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
APPLICATION_INCFLAGS += -I$(BSP_INCDIR)

# Application Files
APPLICATION_SRCS = $(wildcard $(APPLICATION_SRCDIR)/*.c $(APPLICATION_SRCDIR)/*/*.c)
APPLICATION_OBJS = $(subst $(APPLICATION_SRCDIR)/,$(APPLICATION_OBJDIR)/,$(APPLICATION_SRCS:.c=-$(VERSION).o))

# Application compilation
$(APPLICATION_OBJDIR)/%-$(VERSION).o : $(APPLICATION_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(APPLICATION_CFLAGS) $(APPLICATION_INCFLAGS) $(VERSION_FLAGS) $^ -o $@

##############################################
################### CORE #####################
##############################################

# Files
CORE_OBJS = $(MAIN_OBJS) $(APPLICATION_OBJS)

core: $(CORE_OBJS)
	@echo "*******************************"
	@echo "*****   Core Build Done   *****"
	@echo "*******************************"
	@echo