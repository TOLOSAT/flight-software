# OS Building Makefile

##############################################
##################### OS #####################
##############################################

# OS Flags
OS_CFLAGS    = $(PROJECT_CFLAGS) -Wno-unused-variable -Wno-unused-parameter -Wno-pedantic
OS_INCFLAGS  = -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
OS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)

# OS Files
OS_KERNEL_SRCS = $(wildcard $(OS_KERNEL_SRCDIR)/*.c $(OS_KERNEL_ARM_DIR)/*.c $(OS_KERNEL_COMMON_DIR)/*.c $(OS_KERNEL_MEMMANG_DIR)/heap_1.c)
OS_KERNEL_OBJS = $(subst $(OS_KERNEL_SRCDIR)/,$(OS_KERNEL_OBJDIR)/,$(OS_KERNEL_SRCS:.c=-$(BUILD_TYPE).o))
OS_KERNEL_LIB  = $(BUILD_LIBS_DIR)/libos-$(BUILD_TYPE).a

# OS Components compilation
$(OS_KERNEL_OBJDIR)/%-$(BUILD_TYPE).o : $(OS_KERNEL_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(OS_CFLAGS) $(OS_INCFLAGS) $(VERSION_FLAGS) $^ -o $@

# OS Library
$(OS_KERNEL_LIB) : $(OS_KERNEL_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# OS Recipe
os-start :
	@echo "**************************************"
	@echo "*********   OS Start Build   *********"
	@echo "**************************************"

os-end :
	@echo "**************************************"
	@echo "**********   OS Build Done   *********"
	@echo "**************************************"
	@echo

os : os-start $(OS_KERNEL_LIB) os-end
