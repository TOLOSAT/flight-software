# BSP Building Makefile

##############################################
#################### BSP #####################
##############################################

# BSP Flags
BSP_CFLAGS    = $(PROJECT_CFLAGS)
BSP_INCFLAGS  = -I$(BSP_INCDIR)
BSP_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
BSP_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)

# BSP Files
BSP_SRCS = $(wildcard $(BSP_SRCDIR)/*.c $(BSP_SRCDIR)/*.s)
BSP_OBJS = $(patsubst $(BSP_SRCDIR)/%.c,$(BSP_OBJDIR)/%-$(VERSION).o,$(filter %.c,$(BSP_SRCS))) \
           $(patsubst $(BSP_SRCDIR)/%.s,$(BSP_OBJDIR)/%-$(VERSION).o,$(filter %.s,$(BSP_SRCS)))
BSP_LIB  = $(BUILD_LIBS_DIR)/libbsp-$(VERSION).a

# BSP compilation
$(BSP_OBJDIR)/%-$(VERSION).o : $(BSP_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(BSP_CFLAGS) $(BSP_INCFLAGS) $(VERSION_FLAGS) $^ -o $@

$(BSP_OBJDIR)/%-$(VERSION).o : $(BSP_SRCDIR)/%.s
	mkdir -p $(@D)
	$(CC) $(BSP_CFLAGS) $(BSP_INCFLAGS) $(VERSION_FLAGS) $^ -o $@

# BSP Library
$(BSP_LIB) : $(BSP_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^

# BSP Recipe
bsp : $(BSP_LIB)
	@echo "******************************"
	@echo "*****   BSP Build Done   *****"
	@echo "******************************"
	@echo