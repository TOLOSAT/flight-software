# Makefile pour compiler le bsp

##############################################
#################### BSP #####################
##############################################

# BSP Flags
BSP_CFLAGS    = $(GENERIC_CFLAGS)
BSP_INCFLAGS  = -I$(BSP_INCDIR) 
BSP_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)

# BSP Files
BSP_SRCS     = $(wildcard $(BSP_SRCDIR)/*.c)
BSP_DBG_OBJS = $(subst $(BSP_SRCDIR)/,$(BSP_OBJDIR)/,$(BSP_SRCS:.c=-debug.o))
BSP_RLS_OBJS = $(subst $(BSP_SRCDIR)/,$(BSP_OBJDIR)/,$(BSP_SRCS:.c=-release.o))

# BSP compilation
$(BSP_OBJDIR)/%-debug.o : $(BSP_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(BSP_CFLAGS) $(BSP_INCFLAGS) $(GENERIC_DBGFLAGS) $^ -o $@

$(BSP_OBJDIR)/%-release.o : $(BSP_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(BSP_CFLAGS) $(BSP_INCFLAGS) $(GENERIC_RLSFLAGS) $^ -o $@

bsp-dbg : $(BSP_DBG_OBJS)
	@echo "******************************"
	@echo "*****   BSP Build Done   *****"
	@echo "******************************"
	@echo

bsp-rls : $(BSP_RLS_OBJS)
	@echo "******************************"
	@echo "*****   BSP Build Done   *****"
	@echo "******************************"
	@echo