# Application Building Makefile

ifndef BUILD_APPLICATION_MK
BUILD_APPLICATION_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk
include gen/cc_settings.mk

##############################################
################ APPLICATION #################
##############################################

# Application flags
APPLICATION_CFLAGS    = $(PROJECT_CFLAGS)
APPLICATION_INCFLAGS  = -I$(APPLICATION_INCDIR)
APPLICATION_INCFLAGS += -I$(CORE_INCDIR)
APPLICATION_INCFLAGS += -I$(PRE_BUILD_DIR)
APPLICATION_INCFLAGS += -I$(LIBPUS_INCDIR)
APPLICATION_INCFLAGS += -I$(IRIDIUM_DRV_INCDIR)
APPLICATION_INCFLAGS += -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
APPLICATION_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
APPLICATION_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
APPLICATION_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
APPLICATION_INCFLAGS += -I$(BSP_INCDIR)

# Application files
APPLICATION_SRCS = $(wildcard $(APPLICATION_SRCDIR)/*.c $(APPLICATION_SRCDIR)/*/*.c) $(APPLICATION_CONF_SRCS)
APPLICATION_OBJS = $(patsubst $(APPLICATION_SRCDIR)/%.c,$(APPLICATION_OBJDIR)/%-$(BUILD_TYPE).o,$(patsubst $(PRE_BUILD_DIR)/conf/%.c,$(APPLICATION_OBJDIR)/conf/%-$(BUILD_TYPE).o,$(APPLICATION_SRCS)))
APPLICATION_LIB	 = $(LIBS_DIR)/libapplication-$(BUILD_TYPE).a

# Include dependencies
-include $(APPLICATION_OBJS:.o=.d)

# Application recipes
.PHONY += application application-start application-end
application : application-start $(APPLICATION_LIB) application-end

# Build header
application-start :
	@echo "============================="
	@echo "===      APPLICATION      ==="
	@echo "============================="
	@echo "Files to compile: $(words $(APPLICATION_SRCS))"
	@echo "Compilation Flags:"
	@echo $(APPLICATION_CFLAGS)
	@echo "Include Paths:"
	@echo $(APPLICATION_INCFLAGS)
	@echo "Version Flags:"
	@echo $(VERSION_FLAGS)
	@echo "Start building:"
	@$(eval start_time=$(shell date +%s))

# Building recipes
$(APPLICATION_OBJDIR)/%-$(BUILD_TYPE).o : $(APPLICATION_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(APPLICATION_CFLAGS) $(APPLICATION_INCFLAGS) $(VERSION_FLAGS) $< -o $@

$(APPLICATION_OBJDIR)/conf/%-$(BUILD_TYPE).o  : $(PRE_BUILD_DIR)/conf/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(APPLICATION_CFLAGS) $(APPLICATION_INCFLAGS) $(VERSION_FLAGS) $< -o $@

# Library generation
$(APPLICATION_LIB) : $(APPLICATION_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
application-end :
	@$(eval end_time=$(shell date +%s))
	@echo "Build done ($$(($(end_time)-$(start_time))) seconds elapsed)"
	@echo ""

endif # BUILD_APPLICATION_MK #