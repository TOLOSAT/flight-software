# BSP Building Makefile

##############################################
#################### BSP #####################
##############################################

# BSP Flags
BSP_CFLAGS    = $(GENERIC_CFLAGS)
BSP_INCFLAGS  = -I$(BSP_INCDIR)
BSP_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
BSP_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)

# BSP Files
BSP_SRCS = $(wildcard $(BSP_SRCDIR)/*.c)
BSP_OBJS = $(subst $(BSP_SRCDIR)/,$(BSP_OBJDIR)/,$(BSP_SRCS:.c=-$(VERSION).o))

# BSP compilation
$(BSP_OBJDIR)/%-$(VERSION).o : $(BSP_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(BSP_CFLAGS) $(BSP_INCFLAGS) $(VERSION_FLAGS) $^ -o $@

bsp : $(BSP_OBJS)
	@echo "******************************"
	@echo "*****   BSP Build Done   *****"
	@echo "******************************"
	@echo