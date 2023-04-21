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
OS_KERNEL_OBJS = $(subst $(OS_KERNEL_SRCDIR)/,$(OS_KERNEL_OBJDIR)/,$(OS_KERNEL_SRCS:.c=-$(VERSION).o))

OS_CMSIS_SRCS     = $(wildcard $(OS_CMSIS_SRCDIR)/cmsis_os2.c $(OS_CMSIS_SRCDIR)/os_systick.c)
OS_CMSIS_OBJS = $(subst $(OS_CMSIS_SRCDIR)/,$(OS_CMSIS_OBJDIR)/,$(OS_CMSIS_SRCS:.c=-$(VERSION).o))

# OS Components compilation
$(OS_KERNEL_OBJDIR)/%-$(VERSION).o : $(OS_KERNEL_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(OS_CFLAGS) $(OS_INCFLAGS) $(VERSION_FLAGS) $^ -o $@

$(OS_CMSIS_OBJDIR)/%-$(VERSION).o : $(OS_CMSIS_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(OS_CFLAGS) $(OS_INCFLAGS) $(VERSION_FLAGS) $^ -o $@

##############################################
##################### OS #####################
##############################################

OS_OBJS = $(OS_KERNEL_OBJS) $(OS_CMSIS_OBJS)

os : $(OS_OBJS)
	@echo "*****************************"
	@echo "*****   OS Build Done   *****"
	@echo "*****************************"
	@echo