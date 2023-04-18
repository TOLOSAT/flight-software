# Makefile pour compiler le core

##############################################
################### MAIN #####################
##############################################

MAIN_CFLAGS    = $(GENERIC_CFLAGS)
MAIN_INCFLAGS  = -I$(MAIN_INCDIR)
MAIN_INCFLAGS += -I$(CMSIS_INCDIR)
MAIN_INCFLAGS += -I$(CMSIS_INCDIR_DEVICE) -D $(CHIP)
MAIN_INCFLAGS += -I$(HAL_INCDIR)
MAIN_INCFLAGS += -I$(HAL_INCDIR)/Legacy
MAIN_INCFLAGS += -I$(HAL_TOLOSAT_INCDIR)
MAIN_INCFLAGS += -I$(BSP_INCDIR)

# Main Files
MAIN_SRCS = $(wildcard $(MAIN_SRCDIR)/*.c)
MAIN_OBJS = $(MAIN_SRCS:.c=.o)
MAIN_OBJS := $(subst $(MAIN_SRCDIR)/,$(MAIN_OBJDIR)/,$(MAIN_OBJS))

# Main compilation
$(MAIN_OBJDIR)/%.o : $(MAIN_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(MAIN_CFLAGS) $(MAIN_INCFLAGS) $(GENERIC_DBGCFLAGS) $^ -o $@

##############################################
################### CORE #####################
##############################################

# Files
CORE_OBJS = $(MAIN_OBJS)

core : $(CORE_OBJS)
