# Makefile pour compiler le bsp

##############################################
#################### BSP #####################
##############################################

# BSP Directories
ifeq ($(BOARD), NUCLEO-F411RE)
	BSP_DIR = $(BSPs_DIR)/STM32F411RE-bsp
else
	print := $(error This board is not available for the flight software)
endif
BSP_INCDIR = $(BSP_DIR)/inc
BSP_SRCDIR = $(BSP_DIR)/src
BSP_OBJDIR = $(BUILD_TOOLS_DIR)/bsp

# Files
BSP_SRCS = $(wildcard $(BSP_SRCDIR)/*.c)
BSP_OBJS = $(BSP_SRCS:.c=.o)
BSP_OBJS := $(subst $(BSP_SRCDIR)/,$(BSP_OBJDIR)/,$(BSP_OBJS))

LINKER_SCRIPT = $(LINKER_SCRIPTS_DIR)/stm32f4_ls.ld

# BSP compilation
$(BSP_OBJDIR)/%.o : $(BSP_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCFLAGS) $(DBGCFLAGS) $^ -o $@

bsp : $(BSP_OBJS)