# BSP Building Makefile

ifndef BUILD_BSP_MK
BUILD_BSP_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk
include gen/cc_settings.mk

##############################################
#################### BSP #####################
##############################################

# BSP files
BSP_SRCS = $(wildcard $(BSP_SRCDIR)/*.c)
BSP_OBJS = $(patsubst $(BSP_SRCDIR)/%.c,$(BSP_OBJDIR)/%-$(BUILD_TYPE).o,$(BSP_SRCS))
BSP_LIB  = $(LIBS_DIR)/libbsp-$(BUILD_TYPE).a

# BSP flags
BSP_CFLAGS    = $(PROJECT_CFLAGS)
BSP_INCFLAGS  = -I$(BSP_INCDIR)
BSP_INCFLAGS += -I$(CORE_INCDIR)
BSP_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
BSP_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
BSP_INCFLAGS += -I$(PRE_BUILD_DIR)

# Include dependencies
-include $(BSP_OBJS:.o=.d)

# BSP recipes
.PHONY += bsp bsp-start bsp-end bsp-clean
bsp : bsp-start $(BSP_LIB) bsp-end

# Build header
bsp-start :
	@echo "============================="
	@echo "===          BSP          ==="
	@echo "============================="
	@echo "Files to compile: $(words $(BSP_SRCS))"
	@echo "Compilation Flags:"
	@echo $(BSP_CFLAGS)
	@echo "Include Paths:"
	@echo $(BSP_INCFLAGS)
	@echo "Version Flags:"
	@echo $(VERSION_FLAGS)
	@echo "Start building:"

# Building recipes
$(BSP_OBJDIR)/%-$(BUILD_TYPE).o : $(BSP_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(BSP_CFLAGS) $(BSP_INCFLAGS) $(VERSION_FLAGS) $< -o $@

# Library generation
$(BSP_LIB) : $(BSP_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
bsp-end :
	@echo "Build done"
	@echo ""

# Clean recipe
bsp-clean :
	@echo "Cleaning BSP build directory ..."
	@rm -rf $(BSP_OBJDIR)
	@rm -rf $(BSP_LIB)
	@echo "Done"

endif # BUILD_BSP_MK #