# MIDDLEWARES Building Makefile

ifndef BUILD_MIDDLEWARE_MK
BUILD_MIDDLEWARE_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk
include gen/cc_settings.mk

##############################################
################# PUS LIBRARY ################
##############################################

# PUS library files
PUS_SRCS = $(wildcard $(PUS_SRCDIR)/*.c $(PUS_SRCDIR)/*/*.c)
PUS_OBJS = $(subst $(PUS_SRCDIR)/,$(PUS_OBJDIR)/,$(PUS_SRCS:.c=-$(BUILD_TYPE).o))
PUS_LIB  = $(LIBS_DIR)/libpus-$(BUILD_TYPE).a

# PUS LIBRARY flags
PUS_CFLAGS    = $(PROJECT_CFLAGS)
PUS_INCFLAGS  = -I$(PUS_INCDIR)
PUS_INCFLAGS += -I$(KERNEL_INCDIR)
PUS_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
PUS_INCFLAGS += -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
PUS_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
PUS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
PUS_INCFLAGS += -I$(PRE_BUILD_DIR)
PUS_INCFLAGS += -I$(BSP_INCDIR)

# Include dependencies
-include $(PUS_OBJS:.o=.d)

# PUS library recipes
.PHONY : pus pus-start pus-end pus-clean
pus : pus-start $(PUS_LIB) pus-end

# Build header
pus-start :
	@echo "============================="
	@echo "===          PUS          ==="
	@echo "============================="
	@echo "Files to compile: $(words $(PUS_SRCS))"
	@echo "Compilation Flags:"
	@echo $(PUS_CFLAGS)
	@echo "Include Paths:"
	@echo $(PUS_INCFLAGS)
	@echo "Version Flags:"
	@echo $(VERSION_FLAGS)
	@echo "Start building:"

# Building recipes
$(PUS_OBJDIR)/%-$(BUILD_TYPE).o : $(PUS_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(PUS_CFLAGS) $(PUS_INCFLAGS) $(VERSION_FLAGS) $< -o $@

# Library generation
$(PUS_LIB) : $(PUS_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
pus-end :
	@echo "Build done"
	@echo ""

# Clean recipe
pus-clean :
	@echo "Cleaning PUS build directory ..."
	@rm -rf $(PUS_OBJDIR)
	@rm -rf $(PUS_LIB)
	@echo "Done"

##############################################
############### IRIDIUM DRIVER ###############
##############################################

# Iridium driver files
IRIDIUM_SRCS = $(wildcard $(IRIDIUM_SRCDIR)/*.c)
IRIDIUM_OBJS = $(subst $(IRIDIUM_SRCDIR)/,$(IRIDIUM_OBJDIR)/,$(IRIDIUM_SRCS:.c=-$(BUILD_TYPE).o))
IRIDIUM_LIB  = $(LIBS_DIR)/libiridium-$(BUILD_TYPE).a

# Iridium driver flags
IRIDIUM_CFLAGS    = $(PROJECT_CFLAGS)
IRIDIUM_INCFLAGS  = -I$(IRIDIUM_INCDIR)
IRIDIUM_INCFLAGS += -I$(KERNEL_INCDIR)
IRIDIUM_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
IRIDIUM_INCFLAGS += -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
IRIDIUM_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
IRIDIUM_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
IRIDIUM_INCFLAGS += -I$(PRE_BUILD_DIR)
IRIDIUM_INCFLAGS += -I$(BSP_INCDIR)

# Include dependencies
-include $(IRIDIUM_OBJS:.o=.d)

# Iridium Driver recipes
.PHONY : iridium iridium-start iridium-end iridium-clean
iridium : iridium-start $(IRIDIUM_LIB) iridium-end

# Build header
iridium-start :
	@echo "============================="
	@echo "===      IRIDIUM DRV      ==="
	@echo "============================="
	@echo "Files to compile: $(words $(IRIDIUM_SRCS))"
	@echo "Compilation Flags:"
	@echo $(IRIDIUM_CFLAGS)
	@echo "Include Paths:"
	@echo $(IRIDIUM_INCFLAGS)
	@echo "Version Flags:"
	@echo $(VERSION_FLAGS)
	@echo "Start building:"

# Building recipes
$(IRIDIUM_OBJDIR)/%-$(BUILD_TYPE).o : $(IRIDIUM_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(IRIDIUM_CFLAGS) $(IRIDIUM_INCFLAGS) $(VERSION_FLAGS) $< -o $@

# Library generation
$(IRIDIUM_LIB) : $(IRIDIUM_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
iridium-end :
	@echo "Build done"
	@echo ""

# Clean recipe
iridium-clean :
	@echo "Cleaning IRIDIUM build directory ..."
	@rm -rf $(IRIDIUM_OBJDIR)
	@rm -rf $(IRIDIUM_LIB)
	@echo "Done"

endif # BUILD_MIDDLEWARE_MK #