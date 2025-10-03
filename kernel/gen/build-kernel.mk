# Makefile including all build recipes for kernel

ifndef BUILD_KERNEL_MK
BUILD_KERNEL_MK := yes

##############################################
############# DIRECTORIES & FILES ############
##############################################

KERNEL_MODULES = core drv fdir system

# Directories
KERNEL_INCDIR	= $(KERNEL_DIR)
KERNEL_SRCDIR	= $(KERNEL_DIR)
KERNEL_OBJDIR	= $(BUILD_DIR)/kernel
LIBDIR			= $(BUILD_DIR)/libs

# Files
KERNEL_SRCS = $(foreach m,$(KERNEL_MODULES), $(wildcard $(KERNEL_SRCDIR)/$(m)/*.c) $(wildcard $(KERNEL_SRCDIR)/$(m)/*/*.c) $(wildcard $(KERNEL_SRCDIR)/$(m)/*/wrapper-$(CHIP_VENDOR)/*.c)) \
	   		  $(wildcard $(KERNEL_SRCDIR)/bsp/$(BOARD)-BSP/src/*.c) \
	   		  $(SYS_CONF_SRCS) \
	   		  $(BSP_CONF_SRCS)
KERNEL_OBJS = $(patsubst $(KERNEL_SRCDIR)/%.c,$(KERNEL_OBJDIR)/%.o, \
       		  $(patsubst $(PRE_BUILD_DIR)/%.c,$(PRE_BUILD_DIR)/%.o, \
			  $(KERNEL_SRCS)))
KERNEL_LIB 	= $(LIBDIR)/libkernel.a

##############################################
#################### FLAGS ###################
##############################################

# System defines
SYSTEM_DEFINES  = -DSYSTEM_NAME=\"TAPAS\"
SYSTEM_DEFINES += -DPROGRAM_NAME=\"$(PROJ_NAME)\"
SYSTEM_DEFINES += -DMAJOR=$(MAJOR)
SYSTEM_DEFINES += -DMINOR=$(MINOR)
SYSTEM_DEFINES += -DPATCH=$(PATCH)
SYSTEM_DEFINES += -DBOARD=\"$(BOARD)\"

# Flags
KERNEL_CFLAGS   = 	$(CFLAGS) \
				   	-D$(CHIP) -D$(CHIP_FAMILLY) $(CORE_SELECT)
KERNEL_INCDIRS  =	$(KERNEL_INCDIR) $(KERNEL_HEADERS) $(KERNEL_INCDIR)/bsp/$(BOARD)-BSP/ \
					$(FREERTOS_INCLUDES) $(FREERTOS_ARM_DIR) \
					$(HAL_INCDIR) $(HAL_INCDIR)/Legacy \
					$(FATFS_INCDIR) \
					$(CMSIS_INCDIR) $(CMSIS_INCDIR_DEVICE) \
					$(THIRD_PARTIES_CONFDIR) \
					$(PRE_BUILD_DIR)
KERNEL_INCFLAGS = $(addprefix -I,$(KERNEL_INCDIRS))

##############################################
################ BUILD RECIPES ###############
##############################################

.PHONY : kernel kernel-start kernel-end kernel-clean
kernel : pre-build kernel-start $(KERNEL_LIB) kernel-end

# Include dependencies
-include $(KERNEL_OBJS:.o=.d)

# Build header
kernel-start :
	@echo "============================="
	@echo "===         KERNEL        ==="
	@echo "============================="
	@echo "Files to compile: $(words $(KERNEL_SRCS))"
	@echo "Compilation Flags:"
	@echo $(KERNEL_CFLAGS)
	@echo "Include Paths:"
	@$(foreach dir,$(patsubst $(WORKSPACE)/%,%,$(KERNEL_INCDIRS)),echo "  - $(dir)";)
	@echo "Start building:"

# Building recipes
$(KERNEL_OBJDIR)/%.o : $(KERNEL_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(KERNEL_CFLAGS) $(KERNEL_INCFLAGS) $< -o $@

$(PRE_BUILD_DIR)/%.o  : $(PRE_BUILD_DIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(KERNEL_CFLAGS) $(KERNEL_INCFLAGS) $< -o $@

$(KERNEL_OBJDIR)/system/sysinfo.o : $(KERNEL_SRCDIR)/system/sysinfo.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(KERNEL_CFLAGS) $(SYSTEM_DEFINES) $(KERNEL_INCFLAGS) $< -o $@

# Library generation
$(KERNEL_LIB) : $(KERNEL_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
kernel-end :
	@echo "Build done"
	@echo ""

# Clean recipe
kernel-clean :
	@echo "Cleaning KERNEL build directory ..."
	@rm -rf $(KERNEL_OBJDIR)
	@rm -rf $(KERNEL_LIB)
	@echo "Done"

endif # BUILD_KERNEL_MK #