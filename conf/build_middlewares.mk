# MIDDLEWARES Building Makefile

##############################################
################## LIBPUS ####################
##############################################

# LIBPUS Flags
LIBPUS_CFLAGS    = $(GENERIC_CFLAGS)
LIBPUS_INCFLAGS  = -I$(LIBPUS_INCDIR)
LIBPUS_INCFLAGS += -I$(HAL_TOLOSAT_INCDIR)
LIBPUS_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
LIBPUS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
LIBPUS_INCFLAGS += -I$(CMSIS_RTOS2_INCDIR)

# LIBPUS Files
LIBPUS_SRCS = $(wildcard $(LIBPUS_SRCDIR)/*.c $(LIBPUS_SRCDIR)/*/*.c)
LIBPUS_OBJS  = $(subst $(LIBPUS_SRCDIR)/,$(LIBPUS_OBJDIR)/,$(LIBPUS_SRCS:.c=-$(VERSION).o))
LIBPUS_LIB   = $(BUILD_LIBS_DIR)/libpus-$(VERSION).a

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