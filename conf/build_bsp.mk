# Makefile pour compiler le bsp

##############################################
#################### BSP #####################
##############################################

# BSP Flags
BSP_CFLAGS    = $(GENERIC_CFLAGS)
BSP_INCFLAGS  = -I$(BSP_INCDIR) 
BSP_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)

# BSP Files
BSP_SRCS  = $(wildcard $(BSP_SRCDIR)/*.c)
BSP_OBJS  = $(BSP_SRCS:.c=.o)
BSP_OBJS := $(subst $(BSP_SRCDIR)/,$(BSP_OBJDIR)/,$(BSP_OBJS))

# BSP compilation
$(BSP_OBJDIR)/%.o : $(BSP_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(BSP_CFLAGS) $(BSP_INCFLAGS) $(GENERIC_DBGCFLAGS) $^ -o $@

bsp : $(BSP_OBJS)
	@echo "******************************"
	@echo "*****   BSP Build Done   *****"
	@echo "******************************"
	@echo