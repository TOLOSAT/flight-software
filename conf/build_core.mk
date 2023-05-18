# Makefile pour compiler le core

##############################################
################### MAIN #####################
##############################################

# Main Flags
MAIN_CFLAGS    = $(GENERIC_CFLAGS)
MAIN_INCFLAGS  = -I$(MAIN_INCDIR)
MAIN_INCFLAGS += -I$(TASKS_INCDIR)
MAIN_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE) -I$(CMSIS_RTOS2_INCDIR)
MAIN_INCFLAGS += -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
MAIN_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
MAIN_INCFLAGS += -I$(HAL_TOLOSAT_INCDIR)
MAIN_INCFLAGS += -I$(BSP_INCDIR)

# Main Files
MAIN_SRCS = $(wildcard $(MAIN_SRCDIR)/*.c  $(MAIN_SRCDIR)/conf/*.c)
MAIN_OBJS = $(subst $(MAIN_SRCDIR)/,$(MAIN_OBJDIR)/,$(MAIN_SRCS:.c=-$(VERSION).o))

# Main compilation
$(MAIN_OBJDIR)/%-$(VERSION).o : $(MAIN_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(MAIN_CFLAGS) $(MAIN_INCFLAGS) $(VERSION_FLAGS) $^ -o $@

##############################################
################### TASKS ####################
##############################################

# Tasks Flags
TASKS_CFLAGS    = $(GENERIC_CFLAGS)
TASKS_INCFLAGS  = -I$(TASKS_INCDIR)
TASKS_INCFLAGS += -I$(MAIN_INCDIR)
TASKS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE) -I$(CMSIS_RTOS2_INCDIR)
TASKS_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
TASKS_INCFLAGS += -I$(HAL_TOLOSAT_INCDIR)
TASKS_INCFLAGS += -I$(BSP_INCDIR)

# Tasks Files
TASKS_SRCS     = $(wildcard $(TASKS_SRCDIR)/*.c)
TASKS_OBJS = $(subst $(TASKS_SRCDIR)/,$(TASKS_OBJDIR)/,$(TASKS_SRCS:.c=-$(VERSION).o))

# Tasks compilation
$(TASKS_OBJDIR)/%-$(VERSION).o : $(TASKS_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(TASKS_CFLAGS) $(TASKS_INCFLAGS) $(VERSION_FLAGS) $^ -o $@

##############################################
################### CORE #####################
##############################################

# Files
CORE_OBJS = $(MAIN_OBJS) $(TASKS_OBJS)

core: $(CORE_OBJS)
	@echo "*******************************"
	@echo "*****   Core Build Done   *****"
	@echo "*******************************"
	@echo