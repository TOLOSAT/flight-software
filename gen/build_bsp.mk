# BSP Building Makefile

##############################################
#################### BSP #####################
##############################################

# BSP Flags
BSP_CFLAGS    = $(PROJECT_CFLAGS)
BSP_INCFLAGS  = -I$(BSP_INCDIR)
BSP_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
BSP_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)

# BSP Files
BSP_SRCS = $(wildcard $(BSP_SRCDIR)/*.c $(BSP_SRCDIR)/*.s)
BSP_OBJS = $(patsubst $(BSP_SRCDIR)/%.c,$(BUILD_BSP_DIR)/%-$(BUILD_TYPE).o,$(filter %.c,$(BSP_SRCS))) \
           $(patsubst $(BSP_SRCDIR)/%.s,$(BUILD_BSP_DIR)/%-$(BUILD_TYPE).o,$(filter %.s,$(BSP_SRCS)))
BSP_LIB  = $(BUILD_LIBS_DIR)/libbsp-$(BUILD_TYPE).a

# Include dependancies
-include $(BSP_OBJS:.o=.d)

# BSP compilation
$(BUILD_BSP_DIR)/%-$(BUILD_TYPE).o : $(BSP_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(BSP_CFLAGS) $(BSP_INCFLAGS) $(VERSION_FLAGS) $< -o $@

$(BUILD_BSP_DIR)/%-$(BUILD_TYPE).o : $(BSP_SRCDIR)/%.s
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(BSP_CFLAGS) $(BSP_INCFLAGS) $(VERSION_FLAGS) $< -o $@

# BSP Library
$(BSP_LIB) : $(BSP_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# BSP Recipe
bsp-start :
	@echo "**************************************"
	@echo "*********   BSP Start Build   ********"
	@echo "**************************************"

bsp-end :
	@echo "**************************************"
	@echo "*********   BSP Build Done   *********"
	@echo "**************************************"
	@echo

bsp : bsp-start $(BSP_LIB) bsp-end