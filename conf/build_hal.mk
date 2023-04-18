# Makefile pour compiler la HAL

##############################################
#################### HAL #####################
##############################################

HAL_CFLAGS    = $(GENERIC_CFLAGS)
HAL_INCFLAGS  = -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy 
HAL_INCFLAGS += -I$(HAL_TOLOSAT_INCDIR)
HAL_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE) -D $(CHIP)

# HAL Files
HAL_SRCS = $(wildcard $(HAL_SRCDIR)/*.c $(HAL_SRCDIR)/Legacy/*.c)
HAL_SRCS := $(filter-out %_template.c, $(HAL_SRCS))
HAL_OBJS = $(HAL_SRCS:.c=.o)
HAL_OBJS := $(subst $(HAL_SRCDIR)/,$(HAL_OBJDIR)/,$(HAL_OBJS))
HAL_LIB = $(BUILD_LIBS_DIR)/libhal.a

# HAL compilation
$(HAL_OBJDIR)/%.o : $(HAL_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(GENERIC_CFLAGS) $(HAL_INCFLAGS) $(GENERIC_DBGCFLAGS) $^ -o $@ 

# HAL Library
$(HAL_LIB) : $(HAL_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^
	@echo "******************************"
	@echo "*****   HAL Build Done   *****"
	@echo "******************************"
	@echo

libhal : $(HAL_LIB)

##############################################
################ HAL TOLOSAT #################
##############################################

HAL_TOLOSAT_CFLAGS    = $(GENERIC_CFLAGS)
HAL_TOLOSAT_INCFLAGS  = -I$(HAL_TOLOSAT_INCDIR)
HAL_TOLOSAT_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy 
HAL_TOLOSAT_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE) -D $(CHIP)

# HAL TOLOSAT Files
HAL_TOLOSAT_SRCS = $(wildcard $(HAL_TOLOSAT_SRCDIR)/*.c)
HAL_TOLOSAT_OBJS = $(HAL_TOLOSAT_SRCS:.c=.o)
HAL_TOLOSAT_OBJS := $(subst $(HAL_TOLOSAT_SRCDIR)/,$(HAL_TOLOSAT_OBJDIR)/,$(HAL_TOLOSAT_OBJS))
HAL_TOLOSAT_LIB = $(BUILD_LIBS_DIR)/libhal-tolosat.a

# HAL TOLOSAT compilation
$(HAL_TOLOSAT_OBJDIR)/%.o : $(HAL_TOLOSAT_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(GENERIC_CFLAGS) $(HAL_TOLOSAT_INCFLAGS) $(GENERIC_DBGCFLAGS) $^ -o $@ 

# HAL TOLOSAT Library
$(HAL_TOLOSAT_LIB) : $(HAL_TOLOSAT_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^
	@echo "********************************"
	@echo "**   HAL TOLOSAT Build Done   **"
	@echo "********************************"
	@echo

libhal-tolosat : $(HAL_TOLOSAT_LIB)