# Makefile pour compiler le core

##############################################
################### MAIN #####################
##############################################

# Main Flags
MAIN_CFLAGS    = $(GENERIC_CFLAGS)
MAIN_INCFLAGS  = -I$(MAIN_INCDIR)
MAIN_INCFLAGS += -I$(TASKS_INCDIR)
MAIN_INCFLAGS += -I$(CMSIS_INCDIR)
MAIN_INCFLAGS += -I$(CMSIS_INCDIR_DEVICE)
MAIN_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy
MAIN_INCFLAGS += -I$(HAL_TOLOSAT_INCDIR)
MAIN_INCFLAGS += -I$(BSP_INCDIR)
MAIN_INCFLAGS += -I$(CMSIS_RTOS2_INCDIR)

# Main Files
MAIN_SRCS = $(wildcard $(MAIN_SRCDIR)/*.c)
MAIN_DBG_OBJS = $(subst $(MAIN_SRCDIR)/,$(MAIN_OBJDIR)/,$(MAIN_SRCS:.c=-debug.o))
MAIN_RLS_OBJS = $(subst $(MAIN_SRCDIR)/,$(MAIN_OBJDIR)/,$(MAIN_SRCS:.c=-release.o))

# Main compilation
$(MAIN_OBJDIR)/%-debug.o : $(MAIN_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(MAIN_CFLAGS) $(MAIN_INCFLAGS) $(GENERIC_DBGFLAGS) $^ -o $@

$(MAIN_OBJDIR)/%-release.o : $(MAIN_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(MAIN_CFLAGS) $(MAIN_INCFLAGS) $(GENERIC_RLSFLAGS) $^ -o $@

##############################################
################### TASKS ####################
##############################################

# Tasks Flags
TASKS_CFLAGS    = $(GENERIC_CFLAGS)
TASKS_INCFLAGS  = -I$(TASKS_INCDIR)
TASKS_INCFLAGS += -I$(CMSIS_RTOS2_INCDIR)
TASKS_INCFLAGS += -I$(BSP_INCDIR)
TASKS_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy
TASKS_INCFLAGS += -I$(HAL_TOLOSAT_INCDIR)
TASKS_INCFLAGS += -I$(CMSIS_INCDIR)
TASKS_INCFLAGS += -I$(CMSIS_INCDIR_DEVICE)

# Tasks Files
TASKS_SRCS     = $(wildcard $(TASKS_SRCDIR)/*.c)
TASKS_DBG_OBJS = $(subst $(TASKS_SRCDIR)/,$(TASKS_OBJDIR)/,$(TASKS_SRCS:.c=-debug.o))
TASKS_RLS_OBJS = $(subst $(TASKS_SRCDIR)/,$(TASKS_OBJDIR)/,$(TASKS_SRCS:.c=-release.o))

# Tasks compilation
$(TASKS_OBJDIR)/%-debug.o : $(TASKS_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(TASKS_CFLAGS) $(TASKS_INCFLAGS) $(TASKS_DBGCFLAGS) $^ -o $@

$(TASKS_OBJDIR)/%-release.o : $(TASKS_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(TASKS_CFLAGS) $(TASKS_INCFLAGS) $(TASKS_RLSCFLAGS) $^ -o $@

##############################################
################### CORE #####################
##############################################

# Files
CORE_DBG_OBJS = $(MAIN_DBG_OBJS) $(TASKS_DBG_OBJS)
CORE_RLS_OBJS = $(MAIN_RLS_OBJS) $(TASKS_RLS_OBJS)

core-dbg : $(CORE_DBG_OBJS)
	@echo "*******************************"
	@echo "*****   Core Build Done   *****"
	@echo "*******************************"
	@echo

core-rls : $(CORE_RLS_OBJS)
	@echo "*******************************"
	@echo "*****   Core Build Done   *****"
	@echo "*******************************"
	@echo