# Makefile pour compiler la HAL

##############################################
#################### HAL #####################
##############################################

# HAL Directories
ifeq ($(CHIP_FAMILLY), STM32F4xx)
	HAL_DIR = $(HALs_DIR)/HAL-STM32F4
else
	$(error There is no compatible HAL)
endif
HAL_INCDIR = $(HAL_DIR)/Inc
HAL_SRCDIR = $(HAL_DIR)/Src
HAL_OBJDIR = $(BUILD_TOOLS_DIR)/hal

# HAL Files
HAL_SRCS = $(wildcard $(HAL_SRCDIR)/*.c $(HAL_SRCDIR)/Legacy/*.c)
HAL_OBJS = $(HAL_SRCS:.c=.o)
HAL_OBJS := $(subst $(HAL_SRCDIR)/,$(HAL_OBJDIR)/,$(HAL_OBJS))
HAL_LIB = $(BUILD_LIBS_DIR)/libhal.a

# HAL compilation
$(HAL_OBJDIR)/%.o : $(HAL_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCFLAGS) $(DBGCFLAGS) $^ -o $@ 

$(HAL_LIB) : $(HAL_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^
	@echo "******************************"
	@echo "*****   HAL Build Done   *****"
	@echo "******************************"
	@echo

libhal : $(HAL_LIB)

##############################################
################ HAL TOLOSAT #################
##############################################

# HAL Directories
HAL_TOLOSAT_DIR = $(HALs_DIR)/HAL-TOLOSAT
HAL_TOLOSAT_INCDIR = $(HAL_TOLOSAT_DIR)/inc
HAL_TOLOSAT_SRCDIR = $(HAL_TOLOSAT_DIR)/src
HAL_TOLOSAT_OBJDIR = $(BUILD_TOOLS_DIR)/hal_tolosat

# HAL Files
HAL_TOLOSAT_SRCS = $(wildcard $(HAL_TOLOSAT_SRCDIR)/*.c)
HAL_TOLOSAT_OBJS = $(HAL_TOLOSAT_SRCS:.c=.o)
HAL_TOLOSAT_OBJS := $(subst $(HAL_TOLOSAT_SRCDIR)/,$(HAL_TOLOSAT_OBJDIR)/,$(HAL_TOLOSAT_OBJS))
HAL_TOLOSAT_LIB = $(BUILD_LIBS_DIR)/libhal-tolosat.a

# HAL compilation
$(HAL_TOLOSAT_OBJDIR)/%.o : $(HAL_TOLOSAT_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCFLAGS) $(DBGCFLAGS) $^ -o $@ 

$(HAL_TOLOSAT_LIB) : $(HAL_TOLOSAT_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^
	@echo "********************************"
	@echo "**   HAL TOLOSAT Build Done   **"
	@echo "********************************"
	@echo

libhal-tolosat : $(HAL_TOLOSAT_LIB)