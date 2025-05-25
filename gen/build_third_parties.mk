# HAL Building Makefile

ifndef BUILD_THIRD_PARTIES_MK
BUILD_THIRD_PARTIES_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk
include gen/cc_settings.mk

##############################################
#################### HAL #####################
##############################################

# HAL files
include $(CONF_HALS_DIR)/HAL_SRCS_$(CHIP_FAMILLY).mk
HAL_OBJS  = $(subst $(HAL_SRCDIR)/,$(HAL_OBJDIR)/,$(HAL_SRCS:.c=-$(BUILD_TYPE).o))
HAL_LIB   = $(LIBS_DIR)/libhal-$(BUILD_TYPE).a

# HAL flags
HAL_CFLAGS    = $(PROJECT_CFLAGS) -Wno-unused-variable -Wno-unused-parameter
HAL_INCFLAGS  = -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
HAL_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
HAL_INCFLAGS += -I$(PRE_BUILD_DIR)

# Include dependencies
-include $(HAL_OBJS:.o=.d)

# HAL recipes
.PHONY : hal hal-start hal-end hal-clean
hal : hal-start $(HAL_LIB) hal-end

# Build header
hal-start :
	@echo "============================="
	@echo "===          HAL          ==="
	@echo "============================="
	@echo "Files to compile: $(words $(HAL_SRCS))"
	@echo "Compilation Flags:"
	@echo $(HAL_CFLAGS)
	@echo "Include Paths:"
	@echo $(HAL_INCFLAGS)
	@echo "Version Flags:"
	@echo $(VERSION_FLAGS)
	@echo "Start building:"

# Building recipes
$(HAL_OBJDIR)/%-$(BUILD_TYPE).o : $(HAL_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(HAL_CFLAGS) $(HAL_INCFLAGS) $(VERSION_FLAGS) $< -o $@

# Library generation
$(HAL_LIB) : $(HAL_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
hal-end :
	@echo "Build done"
	@echo ""

# Clean recipe
hal-clean :
	@echo "Cleaning HAL build directory ..."
	@rm -rf $(HAL_OBJDIR)
	@rm -rf $(HAL_LIB)
	@echo "Done"

##############################################
################ FATS LIBRARY ################
##############################################

# FATFS files
FATFS_SRCS = $(filter-out $(FATFS_SRCDIR)/ffsystem_%.c, $(wildcard $(FATFS_SRCDIR)/*.c))
FATFS_OBJS  = $(subst $(FATFS_SRCDIR)/,$(FATFS_OBJDIR)/,$(FATFS_SRCS:.c=-$(BUILD_TYPE).o))
FATFS_LIB   = $(LIBS_DIR)/libfatfs-$(BUILD_TYPE).a

# FATFS flags
FATFS_CFLAGS    = $(PROJECT_CFLAGS) -Wno-unused-variable -Wno-unused-parameter -Wno-stringop-overflow -Wno-unused-function
FATFS_INCFLAGS  = -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
FATFS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
FATFS_INCFLAGS += -I$(PRE_BUILD_DIR)

# Include dependencies
-include $(FATFS_OBJS:.o=.d)

# FATFS recipes
.PHONY : fatfs fatfs-start fatfs-end fatfs-clean
fatfs : fatfs-start $(FATFS_LIB) fatfs-end

# Build header
fatfs-start :
	@echo "============================="
	@echo "===         FATFS         ==="
	@echo "============================="
	@echo "Files to compile: $(words $(FATFS_SRCS))"
	@echo "Compilation Flags:"
	@echo $(FATFS_CFLAGS)
	@echo "Include Paths:"
	@echo $(FATFS_INCFLAGS)
	@echo "Version Flags:"
	@echo $(VERSION_FLAGS)
	@echo "Start building:"

# Building recipes
$(FATFS_OBJDIR)/%-$(BUILD_TYPE).o : $(FATFS_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(FATFS_CFLAGS) $(FATFS_INCFLAGS) $(VERSION_FLAGS) $< -o $@

# Library generation
$(FATFS_LIB) : $(FATFS_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
fatfs-end :
	@echo "Build done"
	@echo ""

# Clean recipe
fatfs-clean :
	@echo "Cleaning FATFS build directory ..."
	@rm -rf $(FATFS_OBJDIR)
	@rm -rf $(FATFS_LIB)
	@echo "Done"

##############################################
##################### OS #####################
##############################################

# OS files
OS_KERNEL_SRCS = $(wildcard $(OS_KERNEL_SRCDIR)/*.c $(OS_KERNEL_ARM_DIR)/*.c $(OS_KERNEL_COMMON_DIR)/*.c)
OS_KERNEL_OBJS = $(subst $(OS_KERNEL_SRCDIR)/,$(OS_KERNEL_OBJDIR)/,$(OS_KERNEL_SRCS:.c=-$(BUILD_TYPE).o))
OS_KERNEL_LIB  = $(LIBS_DIR)/libos-$(BUILD_TYPE).a

# OS flags
OS_CFLAGS    = $(PROJECT_CFLAGS) -Wno-unused-variable -Wno-unused-parameter -Wno-pedantic
OS_INCFLAGS  = -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
OS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
OS_INCFLAGS += -I$(PRE_BUILD_DIR)

# Include dependencies
-include $(OS_KERNEL_OBJS:.o=.d)

# OS recipes
.PHONY : os os-start os-end os-clean
os : os-start $(OS_KERNEL_LIB) os-end

# Build header
os-start :
	@echo "============================="
	@echo "===           OS          ==="
	@echo "============================="
	@echo "Files to compile: $(words $(OS_SRCS))"
	@echo "Compilation Flags:"
	@echo $(OS_CFLAGS)
	@echo "Include Paths:"
	@echo $(OS_INCFLAGS)
	@echo "Version Flags:"
	@echo $(VERSION_FLAGS)
	@echo "Start building:"

# Building recipes
$(OS_KERNEL_OBJDIR)/%-$(BUILD_TYPE).o : $(OS_KERNEL_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(OS_CFLAGS) $(OS_INCFLAGS) $(VERSION_FLAGS) $< -o $@

# Library generation
$(OS_KERNEL_LIB) : $(OS_KERNEL_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
os-end :
	@echo "Build done"
	@echo ""

# Clean recipe
os-clean :
	@echo "Cleaning OS build directory ..."
	@rm -rf $(OS_OBJDIR)
	@rm -rf $(OS_LIB)
	@echo "Done"

endif # BUILD_THIRD_PARTIES_MK #