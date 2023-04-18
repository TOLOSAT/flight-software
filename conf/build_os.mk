# Makefile pour compiler l'OS

##############################################
##################### OS #####################
##############################################

# On a besoin de tools/OS/FreeRTOS-Kernel/portable/GCC/ARM_CM4F
#				 CMSIS_RTOS_V2
#				 tools/OS/FreeRTOS-Kernel/include
#				 tools/OS/FreeRTOS-Kernel
#				 tools/OS/FreeRTOS-Kernel/portable/MemMang/heap_1.c

OS_CFLAGS    = $(GENERIC_CFLAGS)
OS_INCFLAGS  = 

# OS Files
OS_SRCS = $(wildcard $(OS_SRCDIR)/*.c)
OS_OBJS = $(OS_SRCS:.c=.o)
OS_OBJS := $(subst $(OS_SRCDIR)/,$(OS_OBJDIR)/,$(OS_OBJS))

# OS compilation
$(OS_OBJDIR)/%.o : $(OS_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(OS_CFLAGS) $(OS_INCFLAGS) $(GENERIC_DBGCFLAGS) $^ -o $@ 

os : $(OS_OBJS)