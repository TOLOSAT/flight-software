# Applications Building Makefile

ifndef BUILD_APPLICATIONS_MK
BUILD_APPLICATIONS_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk
include gen/cc_settings.mk
include $(APPLICATIONS_DIR)/applications.mk

##############################################
################ APPLICATIONS ################
##############################################

# Applications files
APPLICATIONS_SRCS = $(foreach app,$(APPLICATIONS),$(wildcard $(APPLICATIONS_DIR)/$(app)/src/*.c))
APPLICATIONS_INCS = $(foreach app,$(APPLICATIONS),-I$(APPLICATIONS_DIR)/$(app)/inc)
APPLICATIONS_OBJS = $(subst $(APPLICATIONS_DIR)/,$(APPLICATIONS_OBJDIR)/,$(APPLICATIONS_SRCS:.c=-$(BUILD_TYPE).o))
APPLICATIONS_LIB  = $(LIBS_DIR)/libapplications-$(BUILD_TYPE).a

# Applications flags
APPLICATIONS_CFLAGS    = $(PROJECT_CFLAGS)
APPLICATIONS_INCFLAGS  = $(APPLICATIONS_INCS)
APPLICATIONS_INCFLAGS += -I$(KERNEL_INCDIR)
APPLICATIONS_INCFLAGS += -I$(PUS_INCDIR)
APPLICATIONS_INCFLAGS += -I$(IRIDIUMDRV_INCDIR)
APPLICATIONS_INCFLAGS += -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
APPLICATIONS_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
APPLICATIONS_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
APPLICATIONS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
APPLICATIONS_INCFLAGS += -I$(PRE_BUILD_DIR)
APPLICATIONS_INCFLAGS += -I$(BSP_INCDIR)

# Include dependencies
-include $(APPLICATIONS_OBJS:.o=.d)

# Applications recipes
.PHONY : applications applications-start applications-end applications-clean
applications : applications-start $(APPLICATIONS_LIB) applications-end

# Build header
applications-start :
	@echo "============================="
	@echo "===      APPLICATIONS     ==="
	@echo "============================="
	@echo "Files to compile: $(words $(APPLICATIONS_SRCS))"
	@echo "Compilation Flags:"
	@echo $(APPLICATIONS_CFLAGS)
	@echo "Include Paths:"
	@echo $(APPLICATIONS_INCFLAGS)
	@echo "Version Flags:"
	@echo $(VERSION_FLAGS)
	@echo "Start building:"

# Building recipes
$(APPLICATIONS_OBJDIR)/%-$(BUILD_TYPE).o : $(APPLICATIONS_DIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(APPLICATIONS_CFLAGS) $(APPLICATIONS_INCFLAGS) $(VERSION_FLAGS) $< -o $@

# Library generation
$(APPLICATIONS_LIB) : $(APPLICATIONS_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
applications-end :
	@echo "Build done"
	@echo ""

# Clean recipe
applications-clean :
	@echo "Cleaning APPLICATIONS build directory ..."
	@rm -rf $(APPLICATIONS_OBJDIR)
	@rm -rf $(APPLICATIONS_LIB)
	@echo "Done"

endif # BUILD_APPLICATIONS_MK #