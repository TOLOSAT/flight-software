# OS Building Makefile

##############################################
################ OS Components ###############
##############################################

# OS Flags
OS_CFLAGS    = $(GENERIC_CFLAGS) -Wno-unused-variable -Wno-unused-parameter -Wno-pedantic
OS_INCFLAGS  = -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
OS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)

# OS Files
OS_KERNEL_SRCS = $(wildcard $(OS_KERNEL_SRCDIR)/*.c $(OS_KERNEL_ARM_DIR)/*.c $(OS_KERNEL_COMMON_DIR)/*.c $(OS_KERNEL_MEMMANG_DIR)/heap_1.c)
OS_KERNEL_OBJS = $(subst $(OS_KERNEL_SRCDIR)/,$(OS_KERNEL_OBJDIR)/,$(OS_KERNEL_SRCS:.c=-$(VERSION).o))

# OS Components compilation
$(OS_KERNEL_OBJDIR)/%-$(VERSION).o : $(OS_KERNEL_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(OS_CFLAGS) $(OS_INCFLAGS) $(VERSION_FLAGS) $^ -o $@

##############################################
##################### OS #####################
##############################################

OS_OBJS = $(OS_KERNEL_OBJS)

os : $(OS_OBJS)
	@echo "*****************************"
	@echo "*****   OS Build Done   *****"
	@echo "*****************************"
	@echo