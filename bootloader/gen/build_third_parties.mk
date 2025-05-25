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

# FATFS flags
FATFS_CFLAGS    = $(PROJECT_CFLAGS) -Wno-unused-variable -Wno-unused-parameter -Wno-stringop-overflow -Wno-unused-function
FATFS_INCFLAGS  = -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
FATFS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
FATFS_INCFLAGS += -I$(PRE_BUILD_DIR)

# FATFS files
FATFS_SRCS  = $(wildcard $(FATFS_SRCDIR)/*.c)
FATFS_OBJS  = $(subst $(FATFS_SRCDIR)/,$(FATFS_OBJDIR)/,$(FATFS_SRCS:.c=-$(BUILD_TYPE).o))
FATFS_LIB   = $(LIBS_DIR)/libfatfs-$(BUILD_TYPE).a

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
############### USB-OTG LIBRARY ##############
##############################################

# USBOTG flags
USBOTG_CFLAGS    = $(PROJECT_CFLAGS)
USBOTG_INCFLAGS  = -I$(USBOTG_CORE_INCDIR) -I$(USBOTG_MSC_INCDIR) -I$(CONF_USBOTG_DIR)
USBOTG_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
USBOTG_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
USBOTG_INCFLAGS += -I$(PRE_BUILD_DIR)

# USBOTG files
USBOTG_SRCS = $(filter-out %template.c, $(wildcard $(USBOTG_CORE_SRCDIR)/*.c) $(wildcard $(USBOTG_MSC_SRCDIR)/*.c))
USBOTG_OBJS  = $(subst $(USBOTG_DIR)/,$(USBOTG_OBJDIR)/,$(USBOTG_SRCS:.c=-$(BUILD_TYPE).o))
USBOTG_LIB   = $(LIBS_DIR)/libusbotg-$(BUILD_TYPE).a

# Include dependencies
-include $(USBOTG_OBJS:.o=.d)

# USBOTG recipes
.PHONY : usbotg usbotg-start usbotg-end usbotg-clean
usbotg : usbotg-start $(USBOTG_LIB) usbotg-end

# Build header
usbotg-start :
	@echo "============================="
	@echo "===         USBOTG         ==="
	@echo "============================="
	@echo "Files to compile: $(words $(USBOTG_SRCS))"
	@echo "Compilation Flags:"
	@echo $(USBOTG_CFLAGS)
	@echo "Include Paths:"
	@echo $(USBOTG_INCFLAGS)
	@echo "Version Flags:"
	@echo $(VERSION_FLAGS)
	@echo "Start building:"

# Building recipes
$(USBOTG_OBJDIR)/%-$(BUILD_TYPE).o : $(USBOTG_DIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(USBOTG_CFLAGS) $(USBOTG_INCFLAGS) $(VERSION_FLAGS) $< -o $@

# Library generation
$(USBOTG_LIB) : $(USBOTG_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
usbotg-end :
	@echo "Build done"
	@echo ""

# Clean recipe
usbotg-clean :
	@echo "Cleaning USBOTG build directory ..."
	@rm -rf $(USBOTG_OBJDIR)
	@rm -rf $(USBOTG_LIB)
	@echo "Done"

endif # BUILD_THIRD_PARTIES_MK #