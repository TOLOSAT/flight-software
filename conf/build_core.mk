# Makefile pour compiler le core

##############################################
################### MAIN #####################
##############################################

# Main Directories
MAIN_INCDIR = $(MAIN_DIR)/inc
MAIN_SRCDIR = $(MAIN_DIR)/src
MAIN_OBJDIR = $(BUILD_CORE_DIR)/main

# Files
MAIN_SRCS = $(wildcard $(MAIN_SRCDIR)/*.c)
MAIN_OBJS = $(MAIN_SRCS:.c=.o)
MAIN_OBJS := $(subst $(MAIN_SRCDIR)/,$(MAIN_OBJDIR)/,$(MAIN_OBJS))

LINKER_SCRIPT = $(LINKER_SCRIPTS_DIR)/stm32f4_ls.ld

# Main compilation
$(MAIN_OBJDIR)/%.o : $(MAIN_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCFLAGS) $(DBGCFLAGS) $^ -o $@

##############################################
################### CORE #####################
##############################################

# Files
CORE_OBJS = $(MAIN_OBJS)

core : $(CORE_OBJS)
