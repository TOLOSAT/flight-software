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

# BSP flags
BSP_CFLAGS    = $(PROJECT_CFLAGS)
BSP_INCFLAGS  = -I$(BSP_INCDIR)
BSP_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
BSP_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)

# BSP files
BSP_SRCS = $(wildcard $(BSP_SRCDIR)/*.c)
BSP_OBJS = $(patsubst $(BSP_SRCDIR)/%.c,$(BSP_OBJDIR)/%-$(BUILD_TYPE).o,$(BSP_SRCS))
BSP_LIB  = $(LIBS_DIR)/libbsp-$(BUILD_TYPE).a

# Include dependencies
-include $(BSP_OBJS:.o=.d)

# BSP recipes
.PHONY += bsp bsp-start bsp-end
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
	@$(eval start_time=$(shell date +%s))

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
	@$(eval end_time=$(shell date +%s))
	@echo "Build done ($$(($(end_time)-$(start_time))) seconds elapsed)"
	@echo ""

endif # BUILD_BSP_MK #