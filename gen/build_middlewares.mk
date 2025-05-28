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
############### IRIDIUM DRIVER ###############
##############################################

# Iridium driver files
IRIDIUMDRV_SRCS = $(wildcard $(IRIDIUMDRV_SRCDIR)/*.c)
IRIDIUMDRV_OBJS = $(subst $(IRIDIUMDRV_SRCDIR)/,$(IRIDIUMDRV_OBJDIR)/,$(IRIDIUMDRV_SRCS:.c=-$(BUILD_TYPE).o))
IRIDIUMDRV_LIB  = $(LIBS_DIR)/libiridiumdrv-$(BUILD_TYPE).a

# Iridium driver flags
IRIDIUMDRV_CFLAGS    = $(PROJECT_CFLAGS)
IRIDIUMDRV_INCFLAGS  = -I$(IRIDIUMDRV_INCDIR)
IRIDIUMDRV_INCFLAGS += -I$(KERNEL_INCDIR)
IRIDIUMDRV_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
IRIDIUMDRV_INCFLAGS += -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
IRIDIUMDRV_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
IRIDIUMDRV_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
IRIDIUMDRV_INCFLAGS += -I$(PRE_BUILD_DIR)
IRIDIUMDRV_INCFLAGS += -I$(BSP_INCDIR)

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


##############################################
################# EPS LIBRARY ################
##############################################

# EPS library files
EPS_SRCS = $(wildcard $(EPS_SRCDIR)/*.c $(EPS_SRCDIR)/*/*.c)
EPS_OBJS = $(subst $(EPS_SRCDIR)/,$(EPS_OBJDIR)/,$(EPS_SRCS:.c=-$(BUILD_TYPE).o))
EPS_LIB  = $(LIBS_DIR)/libeps-$(BUILD_TYPE).a

# EPS LIBRARY flags
EPS_CFLAGS    = $(PROJECT_CFLAGS)
EPS_INCFLAGS  = -I$(EPS_INCDIR)
EPS_INCFLAGS += -I$(KERNEL_INCDIR)
EPS_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
EPS_INCFLAGS += -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
EPS_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
EPS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
EPS_INCFLAGS += -I$(PRE_BUILD_DIR)
EPS_INCFLAGS += -I$(BSP_INCDIR)

# Include dependencies
-include $(EPS_OBJS:.o=.d)

# EPS library recipes
.PHONY += eps eps-start eps-end eps-clean
eps : eps-start $(EPS_LIB) eps-end

# Build header
eps-start :
	@echo "============================="
	@echo "===          EPS          ==="
	@echo "============================="
	@echo "Files to compile: $(words $(EPS_SRCS))"
	@echo "Compilation Flags:"
	@echo $(EPS_CFLAGS)
	@echo "Include Paths:"
	@echo $(EPS_INCFLAGS)
	@echo "Version Flags:"
	@echo $(VERSION_FLAGS)
	@echo "Start building:"

# Building recipes
$(EPS_OBJDIR)/%-$(BUILD_TYPE).o : $(EPS_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(EPS_CFLAGS) $(EPS_INCFLAGS) $(VERSION_FLAGS) $< -o $@

# Library generation
$(EPS_LIB) : $(EPS_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
eps-end :
	@echo "Build done"
	@echo ""

# Clean recipe
eps-clean :
	@echo "Cleaning EPS build directory ..."
	@rm -rf $(EPS_OBJDIR)
	@rm -rf $(EPS_LIB)
	@echo "Done"


endif # BUILD_MIDDLEWARE_MK #