# Makefile pour compiler le bsp

##############################################
#################### BSP #####################
##############################################

BSP_CFLAGS   = $(GENERIC_CFLAGS)
BSP_INCFLAGS = -I$(BSP_INCDIR)

# BSP compilation
$(BSP_OBJDIR)/%.o : $(BSP_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(BSP_CFLAGS) $(BSP_INCFLAGS) $(GENERIC_DBGCFLAGS) $^ -o $@

bsp : $(BSP_OBJS)