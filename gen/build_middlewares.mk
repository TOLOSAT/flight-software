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
################## LIBPUS ####################
##############################################

# LIBPUS flags
PUS_CFLAGS    = $(PROJECT_CFLAGS) -DLPUS_EXTERNAL_TIME_MGMT
PUS_INCFLAGS  = -I$(PUS_INCDIR)
PUS_INCFLAGS += -I$(CORE_INCDIR)
PUS_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
PUS_INCFLAGS += -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
PUS_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
PUS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
PUS_INCFLAGS += -I$(PRE_BUILD_DIR)
PUS_INCFLAGS += -I$(BSP_INCDIR)

# LIBPUS files
PUS_SRCS = $(wildcard $(PUS_SRCDIR)/*.c $(PUS_SRCDIR)/*/*.c)
PUS_OBJS = $(subst $(PUS_SRCDIR)/,$(PUS_OBJDIR)/,$(PUS_SRCS:.c=-$(BUILD_TYPE).o))
PUS_LIB  = $(LIBS_DIR)/libpus-$(BUILD_TYPE).a

# Include dependencies
-include $(PUS_OBJS:.o=.d)

# Libpus recipes
.PHONY += pus pus-start pus-end pus-clean
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
############### Iridium Driver ###############
##############################################

# IRIDIUM_DRV flags
IRIDIUMDRV_CFLAGS    = $(PROJECT_CFLAGS)
IRIDIUMDRV_INCFLAGS  = -I$(IRIDIUMDRV_INCDIR)
IRIDIUMDRV_INCFLAGS += -I$(CORE_INCDIR) 
IRIDIUMDRV_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
IRIDIUMDRV_INCFLAGS += -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
IRIDIUMDRV_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
IRIDIUMDRV_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
IRIDIUMDRV_INCFLAGS += -I$(PRE_BUILD_DIR)
IRIDIUMDRV_INCFLAGS += -I$(BSP_INCDIR)

# IRIDIUM_DRV files
IRIDIUMDRV_SRCS = $(wildcard $(IRIDIUMDRV_SRCDIR)/*.c)
IRIDIUMDRV_OBJS = $(subst $(IRIDIUMDRV_SRCDIR)/,$(IRIDIUMDRV_OBJDIR)/,$(IRIDIUMDRV_SRCS:.c=-$(BUILD_TYPE).o))
IRIDIUMDRV_LIB  = $(LIBS_DIR)/libiridiumdrv-$(BUILD_TYPE).a

# Include dependencies
-include $(IRIDIUMDRV_OBJS:.o=.d)

# Iridium Driver recipes
.PHONY += iridiumdrv iridiumdrv-start iridiumdrv-end iridiumdrv-clean
iridiumdrv : iridiumdrv-start $(IRIDIUMDRV_LIB) iridiumdrv-end

# Build header
iridiumdrv-start :
	@echo "============================="
	@echo "===      IRIDIUM DRV      ==="
	@echo "============================="
	@echo "Files to compile: $(words $(IRIDIUMDRV_SRCS))"
	@echo "Compilation Flags:"
	@echo $(IRIDIUMDRV_CFLAGS)
	@echo "Include Paths:"
	@echo $(IRIDIUMDRV_INCFLAGS)
	@echo "Version Flags:"
	@echo $(VERSION_FLAGS)
	@echo "Start building:"

# Building recipes
$(IRIDIUMDRV_OBJDIR)/%-$(BUILD_TYPE).o : $(IRIDIUMDRV_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(IRIDIUMDRV_CFLAGS) $(IRIDIUMDRV_INCFLAGS) $(VERSION_FLAGS) $< -o $@ 

# Library generation
$(IRIDIUMDRV_LIB) : $(IRIDIUMDRV_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
iridiumdrv-end :
	@echo "Build done"
	@echo ""

# Clean recipe
iridiumdrv-clean :
	@echo "Cleaning IRIDIUMDRV build directory ..."
	@rm -rf $(IRIDIUMDRV_OBJDIR)
	@rm -rf $(IRIDIUMDRV_LIB)
	@echo "Done"

endif # BUILD_MIDDLEWARE_MK #