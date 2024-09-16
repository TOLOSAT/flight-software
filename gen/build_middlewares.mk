# MIDDLEWARES Building Makefile

include gen/settings.mk
include gen/path.mk
include gen/cc_settings.mk

##############################################
################## LIBPUS ####################
##############################################

# LIBPUS flags
LIBPUS_CFLAGS    = $(PROJECT_CFLAGS) -DLPUS_EXTERNAL_TIME_MGMT
LIBPUS_INCFLAGS  = -I$(LIBPUS_INCDIR)
LIBPUS_INCFLAGS += -I$(PRE_BUILD_DIR)
LIBPUS_INCFLAGS += -I$(CORE_INCDIR)
LIBPUS_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
LIBPUS_INCFLAGS += -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
LIBPUS_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
LIBPUS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
LIBPUS_INCFLAGS += -I$(BSP_INCDIR)

# LIBPUS files
LIBPUS_SRCS = $(wildcard $(LIBPUS_SRCDIR)/*.c $(LIBPUS_SRCDIR)/*/*.c)
LIBPUS_OBJS = $(subst $(LIBPUS_SRCDIR)/,$(LIBPUS_OBJDIR)/,$(LIBPUS_SRCS:.c=-$(BUILD_TYPE).o))
LIBPUS_LIB  = $(BUILD_LIBS_DIR)/libpus-$(BUILD_TYPE).a

# Include dependencies
-include $(LIBPUS_OBJS:.o=.d)

# Libpus recipes
.PHONY += pus pus-start pus-end
pus : pus-start $(LIBPUS_LIB) pus-end

# Build header
pus-start :
	@echo "============================="
	@echo "===          PUS          ==="
	@echo "============================="
	@echo "Files to compile: $(words $(LIBPUS_SRCS))"
	@echo "Compilation Flags:"
	@echo $(LIBPUS_CFLAGS)
	@echo "Include Paths:"
	@echo $(LIBPUS_INCFLAGS)
	@echo "Version Flags:"
	@echo $(VERSION_FLAGS)
	@echo "Start building:"
	@$(eval start_time=$(shell date +%s))

# Building recipes
$(LIBPUS_OBJDIR)/%-$(BUILD_TYPE).o : $(LIBPUS_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(LIBPUS_CFLAGS) $(LIBPUS_INCFLAGS) $(VERSION_FLAGS) $< -o $@ 

# Library generation
$(LIBPUS_LIB) : $(LIBPUS_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
pus-end :
	@$(eval end_time=$(shell date +%s))
	@echo "Build done ($$(($(end_time)-$(start_time))) seconds elapsed)"
	@echo ""

##############################################
############### Iridium Driver ###############
##############################################

# IRIDIUM_DRV flags
IRIDIUM_DRV_CFLAGS    = $(PROJECT_CFLAGS)
IRIDIUM_DRV_INCFLAGS  = -I$(IRIDIUM_DRV_INCDIR)
IRIDIUM_DRV_INCFLAGS += -I$(CORE_INCDIR)
IRIDIUM_DRV_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
IRIDIUM_DRV_INCFLAGS += -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
IRIDIUM_DRV_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
IRIDIUM_DRV_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE) 
IRIDIUM_DRV_INCFLAGS += -I$(BSP_INCDIR)

# IRIDIUM_DRV files
IRIDIUM_DRV_SRCS = $(wildcard $(IRIDIUM_DRV_SRCDIR)/*.c)
IRIDIUM_DRV_OBJS = $(subst $(IRIDIUM_DRV_SRCDIR)/,$(IRIDIUM_DRV_OBJDIR)/,$(IRIDIUM_DRV_SRCS:.c=-$(BUILD_TYPE).o))
IRIDIUM_DRV_LIB  = $(BUILD_LIBS_DIR)/libiridiumdrv-$(BUILD_TYPE).a

# Include dependencies
-include $(IRIDIUM_DRV_OBJS:.o=.d)

# Iridium Driver recipes
.PHONY += iridiumdrv iridiumdrv-start iridiumdrv-end
iridiumdrv : iridiumdrv-start $(IRIDIUM_DRV_LIB) iridiumdrv-end

# Build header
iridiumdrv-start :
	@echo "============================="
	@echo "===      IRIDIUM DRV      ==="
	@echo "============================="
	@echo "Files to compile: $(words $(IRIDIUM_DRV_SRCS))"
	@echo "Compilation Flags:"
	@echo $(IRIDIUM_DRV_CFLAGS)
	@echo "Include Paths:"
	@echo $(IRIDIUM_DRV_INCFLAGS)
	@echo "Version Flags:"
	@echo $(VERSION_FLAGS)
	@echo "Start building:"
	@$(eval start_time=$(shell date +%s))

# Building recipes
$(IRIDIUM_DRV_OBJDIR)/%-$(BUILD_TYPE).o : $(IRIDIUM_DRV_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(IRIDIUM_DRV_CFLAGS) $(IRIDIUM_DRV_INCFLAGS) $(VERSION_FLAGS) $< -o $@ 

# Library generation
$(IRIDIUM_DRV_LIB) : $(IRIDIUM_DRV_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
iridiumdrv-end :
	@$(eval end_time=$(shell date +%s))
	@echo "Build done ($$(($(end_time)-$(start_time))) seconds elapsed)"
	@echo ""
