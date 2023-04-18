# Makefile pour compiler le cmsis

##############################################
################### CMSIS #####################
##############################################

CMSIS_CFLAGS   = $(GENERIC_CFLAGS)
CMSIS_INCFLAGS = -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE) -D $(CHIP)

# CMSIS Files
CMSIS_SRCS = $(wildcard $(CMSIS_SRCDIR_DEVICE)/*.c)
CMSIS_OBJS = $(CMSIS_SRCS:.c=.o)
CMSIS_OBJS := $(subst $(CMSIS_SRCDIR_DEVICE)/,$(CMSIS_OBJDIR)/,$(CMSIS_OBJS))
CMSIS_LIB = $(BUILD_LIBS_DIR)/libcmsis.a

# CMSIS compilation
$(CMSIS_OBJDIR)/%.o : $(CMSIS_SRCDIR_DEVICE)/%.c
	mkdir -p $(@D)
	$(CC) $(CMSIS_CFLAGS) $(CMSIS_INCFLAGS) $(GENERIC_DBGCFLAGS) $^ -o $@ 

# CMSIS Library
$(CMSIS_LIB) : $(CMSIS_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^
	@echo "******************************"
	@echo "****   CMSIS Build Done   ****"
	@echo "******************************"
	@echo

libcmsis : $(CMSIS_LIB)