# Makefile pour compiler le cmsis

##############################################
################### CMSIS #####################
##############################################

CMSIS_CFLAGS   = $(GENERIC_CFLAGS)
CMSIS_INCFLAGS = -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE) -D $(CHIP)

# CMSIS compilation
$(CMSIS_OBJDIR)/%.o : $(CMSIS_SRCDIR_DEVICE)/%.c
	mkdir -p $(@D)
	$(CC) $(CMSIS_CFLAGS) $(CMSIS_INCFLAGS) $(GENERIC_DBGCFLAGS) $^ -o $@ 

$(CMSIS_LIB) : $(CMSIS_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^
	@echo "******************************"
	@echo "****   CMSIS Build Done   ****"
	@echo "******************************"
	@echo

libcmsis : $(CMSIS_LIB)