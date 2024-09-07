# Core Building Makefile

##############################################
################### CORE #####################
##############################################

# Main Flags
CORE_CFLAGS    = $(PROJECT_CFLAGS)
CORE_INCFLAGS  = -I$(CORE_INCDIR)
CORE_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
CORE_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
CORE_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE) 
CORE_INCFLAGS += -I$(BSP_INCDIR)

# Main Files
CORE_SRCS = $(wildcard $(CORE_SRCDIR)/*.c $(CORE_SRCDIR)/*/*.c)
CORE_OBJS = $(subst $(CORE_SRCDIR)/,$(BUILD_CORE_DIR)/,$(CORE_SRCS:.c=-$(BUILD_TYPE).o))
CORE_LIB  = $(BUILD_LIBS_DIR)/libcore-$(BUILD_TYPE).a

# Main compilation
$(BUILD_CORE_DIR)/%-$(BUILD_TYPE).o : $(CORE_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(CORE_CFLAGS) $(CORE_INCFLAGS) $(VERSION_FLAGS) $^ -o $@

# Core Library
$(CORE_LIB) : $(CORE_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Core Recipe
core-start :
	@echo "**************************************"
	@echo "********   CORE Start Build   ********"
	@echo "**************************************"

core-end :
	@echo "**************************************"
	@echo "*********   CORE Build Done   ********"
	@echo "**************************************"
	@echo

core: core-start $(CORE_LIB) core-end