# Makefile pour compiler le cmsis

##############################################
################### CMSIS #####################
##############################################

# CMSIS Directories
CMSIS_INCDIR = $(CMSIS_DIR)/Include
CMSIS_INCDIR_DEVICE = $(CMSIS_DIR)/Device/$(CHIP_VENDOR)/$(CHIP_FAMILLY)/Include
CMSIS_SRCDIR_DEVICE = $(CMSIS_DIR)/Device/$(CHIP_VENDOR)/$(CHIP_FAMILLY)/Source
CMSIS_OBJDIR = $(BUILD_TOOLS_DIR)/cmsis

# CMSIS Files
CMSIS_SRCS = $(wildcard $(CMSIS_SRCDIR_DEVICE)/*.c)
CMSIS_OBJS = $(CMSIS_SRCS:.c=.o)
CMSIS_OBJS := $(subst $(CMSIS_SRCDIR_DEVICE)/,$(CMSIS_OBJDIR)/,$(CMSIS_OBJS))
CMSIS_LIB = $(BUILD_LIBS_DIR)/libcmsis.a

# CMSIS compilation
$(CMSIS_OBJDIR)/%.o : $(CMSIS_SRCDIR_DEVICE)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCFLAGS) $(DBGCFLAGS) $^ -o $@ 

$(CMSIS_LIB) : $(CMSIS_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^
	@echo "******************************"
	@echo "****   CMSIS Build Done   ****"
	@echo "******************************"
	@echo

libcmsis : $(CMSIS_LIB)