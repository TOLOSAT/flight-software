# Makefile pour compiler la HAL

##############################################
#################### HAL #####################
##############################################

# HAL Flags
HAL_CFLAGS    = $(GENERIC_CFLAGS)
HAL_INCFLAGS  = -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy 
HAL_INCFLAGS += -I$(HAL_TOLOSAT_INCDIR)
HAL_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)

# HAL Files
HAL_SRCS     = $(wildcard $(HAL_SRCDIR)/*.c $(HAL_SRCDIR)/Legacy/*.c)
HAL_SRCS    := $(filter-out %_template.c, $(HAL_SRCS))
HAL_DBG_OBJS = $(subst $(HAL_SRCDIR)/,$(HAL_OBJDIR)/,$(HAL_SRCS:.c=-debug.o))
HAL_RLS_OBJS = $(subst $(HAL_SRCDIR)/,$(HAL_OBJDIR)/,$(HAL_SRCS:.c=-release.o))
HAL_DBG_LIB  = $(BUILD_LIBS_DIR)/libhal-debug.a
HAL_RLS_LIB  = $(BUILD_LIBS_DIR)/libhal-release.a

# HAL compilation
$(HAL_OBJDIR)/%-debug.o : $(HAL_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(HAL_CFLAGS) $(HAL_INCFLAGS) $(GENERIC_DBGFLAGS) $^ -o $@ 

$(HAL_OBJDIR)/%-release.o : $(HAL_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(HAL_CFLAGS) $(HAL_INCFLAGS) $(GENERIC_RLSFLAGS) $^ -o $@ 

# HAL Library
$(HAL_DBG_LIB) : $(HAL_DBG_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^

$(HAL_RLS_LIB) : $(HAL_RLS_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^

libhal-dbg : $(HAL_DBG_LIB)
	@echo "******************************"
	@echo "*****   HAL Build Done   *****"
	@echo "******************************"
	@echo

libhal-rls : $(HAL_RLS_LIB)
	@echo "******************************"
	@echo "*****   HAL Build Done   *****"
	@echo "******************************"
	@echo

##############################################
################ HAL TOLOSAT #################
##############################################

# HAL TOLOSAT Flags
HAL_TOLOSAT_CFLAGS    = $(GENERIC_CFLAGS)
HAL_TOLOSAT_INCFLAGS  = -I$(HAL_TOLOSAT_INCDIR)
HAL_TOLOSAT_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy 
HAL_TOLOSAT_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)

# HAL TOLOSAT Files
HAL_TOLOSAT_SRCS     = $(wildcard $(HAL_TOLOSAT_SRCDIR)/*.c)
HAL_TOLOSAT_DBG_OBJS = $(subst $(HAL_TOLOSAT_SRCDIR)/,$(HAL_TOLOSAT_OBJDIR)/,$(HAL_TOLOSAT_SRCS:.c=-debug.o))
HAL_TOLOSAT_RLS_OBJS = $(subst $(HAL_TOLOSAT_SRCDIR)/,$(HAL_TOLOSAT_OBJDIR)/,$(HAL_TOLOSAT_SRCS:.c=-release.o))
HAL_TOLOSAT_DBG_LIB  = $(BUILD_LIBS_DIR)/libhal-tolosat-debug.a
HAL_TOLOSAT_RLS_LIB  = $(BUILD_LIBS_DIR)/libhal-tolosat-release.a

# HAL TOLOSAT compilation
$(HAL_TOLOSAT_OBJDIR)/%-debug.o : $(HAL_TOLOSAT_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(HAL_TOLOSAT_CFLAGS) $(HAL_TOLOSAT_INCFLAGS) $(GENERIC_DBGFLAGS) $^ -o $@ 

$(HAL_TOLOSAT_OBJDIR)/%-release.o : $(HAL_TOLOSAT_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(HAL_TOLOSAT_CFLAGS) $(HAL_TOLOSAT_INCFLAGS) $(GENERIC_RLSFLAGS) $^ -o $@ 

# HAL TOLOSAT Library
$(HAL_TOLOSAT_DBG_LIB) : $(HAL_TOLOSAT_DBG_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^

$(HAL_TOLOSAT_RLS_LIB) : $(HAL_TOLOSAT_RLS_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^

libhal-tolosat-dbg : $(HAL_TOLOSAT_DBG_LIB)
	@echo "********************************"
	@echo "**   HAL TOLOSAT Build Done   **"
	@echo "********************************"
	@echo

libhal-tolosat-rls : $(HAL_TOLOSAT_RLS_LIB)
	@echo "********************************"
	@echo "**   HAL TOLOSAT Build Done   **"
	@echo "********************************"
	@echo