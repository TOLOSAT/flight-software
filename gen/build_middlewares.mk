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
################# CSP LIBRARY ################
##############################################

# CSP library files
CSP_SRCS = $(wildcard $(CSP_SRCDIR)/*.c $(CSP_SRCDIR)/*/*.c)
CSP_OBJS = $(subst $(CSP_SRCDIR)/,$(CSP_OBJDIR)/,$(CSP_SRCS:.c=-$(BUILD_TYPE).o))
CSP_LIB  = $(LIBS_DIR)/libcsp-$(BUILD_TYPE).a

# CSP LIBRARY flags
CSP_CFLAGS    = $(PROJECT_CFLAGS)
CSP_INCFLAGS  = -I$(CSP_INCDIR)
CSP_INCFLAGS += -I$(KERNEL_INCDIR)
CSP_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
CSP_INCFLAGS += -I$(OS_KERNEL_INCDIR) -I$(OS_KERNEL_ARM_DIR) -I$(CONF_FREERTOS_DIR)
CSP_INCFLAGS += -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
CSP_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
CSP_INCFLAGS += -I$(PRE_BUILD_DIR)
CSP_INCFLAGS += -I$(BSP_INCDIR)

# Include dependencies
-include $(CSP_OBJS:.o=.d)

# CSP library recipes
.PHONY += csp csp-start csp-end csp-clean
csp : csp-start $(CSP_LIB) csp-end

# Build header
csp-start :
	@echo "============================="
	@echo "===          CSP          ==="
	@echo "============================="
	@echo "Files to compile: $(words $(CSP_SRCS))"
	@echo "Compilation Flags:"
	@echo $(CSP_CFLAGS)
	@echo "Include Paths:"
	@echo $(CSP_INCFLAGS)
	@echo "Version Flags:"
	@echo $(VERSION_FLAGS)
	@echo "Start building:"

# Building recipes
$(CSP_OBJDIR)/%-$(BUILD_TYPE).o : $(CSP_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(CSP_CFLAGS) $(CSP_INCFLAGS) $(VERSION_FLAGS) $< -o $@

# Library generation
$(CSP_LIB) : $(CSP_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
csp-end :
	@echo "Build done"
	@echo ""

# Clean recipe
csp-clean :
	@echo "Cleaning CSP build directory ..."
	@rm -rf $(CSP_OBJDIR)
	@rm -rf $(CSP_LIB)
	@echo "Done"


endif # BUILD_MIDDLEWARE_MK #