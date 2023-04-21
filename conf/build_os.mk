# Makefile pour compiler l'OS

##############################################
################ OS Components ###############
##############################################

# OS Flags
OS_CFLAGS    = $(GENERIC_CFLAGS)
OS_INCFLAGS  = -I$(OS_KERNEL_INCDIR)
OS_INCFLAGS += -I$(OS_KERNEL_ARM_DIR)
OS_INCFLAGS += -I$(OS_CMSIS_RTOSV2_DIR)/Include -I$(OS_CMSIS_RTOSV2_DIR)/Include1 -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE) -I$(CMSIS_RTOS2_INCDIR)
OS_INCFLAGS += -I$(CONF_FREERTOS_DIR)

# OS Files
OS_KERNEL_SRCS     = $(wildcard $(OS_KERNEL_SRCDIR)/*.c $(OS_KERNEL_ARM_DIR)/*.c $(OS_KERNEL_MEMMANG_DIR)/heap_4.c)
OS_KERNEL_DBG_OBJS = $(subst $(OS_KERNEL_SRCDIR)/,$(OS_KERNEL_OBJDIR)/,$(OS_KERNEL_SRCS:.c=-debug.o))
OS_KERNEL_RLS_OBJS = $(subst $(OS_KERNEL_SRCDIR)/,$(OS_KERNEL_OBJDIR)/,$(OS_KERNEL_SRCS:.c=-release.o))

OS_CMSIS_SRCS     = $(wildcard $(OS_CMSIS_SRCDIR)/cmsis_os2.c $(OS_CMSIS_SRCDIR)/os_systick.c)
OS_CMSIS_DBG_OBJS = $(subst $(OS_CMSIS_SRCDIR)/,$(OS_CMSIS_OBJDIR)/,$(OS_CMSIS_SRCS:.c=-debug.o))
OS_CMSIS_RLS_OBJS = $(subst $(OS_CMSIS_SRCDIR)/,$(OS_CMSIS_OBJDIR)/,$(OS_CMSIS_SRCS:.c=-release.o))

# OS Components compilation
$(OS_KERNEL_OBJDIR)/%-debug.o : $(OS_KERNEL_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(OS_CFLAGS) $(OS_INCFLAGS) $(GENERIC_DBGFLAGS) $^ -o $@

$(OS_KERNEL_OBJDIR)/%-release.o : $(OS_KERNEL_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(OS_CFLAGS) $(OS_INCFLAGS) $(GENERIC_RLSFLAGS) $^ -o $@

$(OS_CMSIS_OBJDIR)/%-debug.o : $(OS_CMSIS_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(OS_CFLAGS) $(OS_INCFLAGS) $(GENERIC_DBGFLAGS) $^ -o $@

$(OS_CMSIS_OBJDIR)/%-release.o : $(OS_CMSIS_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(OS_CFLAGS) $(OS_INCFLAGS) $(GENERIC_RLSFLAGS) $^ -o $@

##############################################
##################### OS #####################
##############################################

OS_DBG_OBJS = $(OS_KERNEL_DBG_OBJS) $(OS_CMSIS_DBG_OBJS)
OS_RLS_OBJS = $(OS_KERNEL_RLS_OBJS) $(OS_CMSIS_RLS_OBJS)

os-dbg : $(OS_DBG_OBJS)
	@echo "*****************************"
	@echo "*****   OS Build Done   *****"
	@echo "*****************************"
	@echo

os-rls : $(OS_RLS_OBJS)
	@echo "*****************************"
	@echo "*****   OS Build Done   *****"
	@echo "*****************************"
	@echo