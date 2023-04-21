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
MAIN_SRCS  = $(wildcard $(MAIN_SRCDIR)/*.c)
MAIN_OBJS  = $(MAIN_SRCS:.c=.o)
MAIN_OBJS := $(subst $(MAIN_SRCDIR)/,$(MAIN_OBJDIR)/,$(MAIN_OBJS))

# Main compilation
$(MAIN_OBJDIR)/%.o : $(MAIN_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(MAIN_CFLAGS) $(MAIN_INCFLAGS) $(GENERIC_DBGCFLAGS) $^ -o $@

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
TASKS_SRCS  = $(wildcard $(TASKS_SRCDIR)/*.c)
TASKS_OBJS  = $(TASKS_SRCS:.c=.o)
TASKS_OBJS := $(subst $(TASKS_SRCDIR)/,$(TASKS_OBJDIR)/,$(TASKS_OBJS))

# Tasks compilation
$(TASKS_OBJDIR)/%.o : $(TASKS_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(TASKS_CFLAGS) $(TASKS_INCFLAGS) $(TASKS_DBGCFLAGS) $^ -o $@

##############################################
################### CORE #####################
##############################################

# Files
CORE_OBJS = $(MAIN_OBJS) $(TASKS_OBJS)

core : $(CORE_OBJS)
	@echo "*******************************"
	@echo "*****   Core Build Done   *****"
	@echo "*******************************"
	@echo
