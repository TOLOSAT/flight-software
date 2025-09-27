# Kernel Building Makefile

ifndef BUILD_KERNEL_MK
BUILD_KERNEL_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk
include gen/cc_settings.mk

##############################################
############# KERNEL DIRECTORIES #############
##############################################

# Kernel Directories
KERNEL_INCDIR			= $(KERNEL_DIR)
KERNEL_SRCDIR			= $(KERNEL_DIR)
KERNEL_OBJDIR			= $(BUILD_DIR)/kernel
THIRD_PARTIES_DIR		= $(KERNEL_DIR)/third-parties
THIRD_PARTIES_OBJDIR	= $(KERNEL_OBJDIR)/third-parties

# Third parties list
KERNEL_THIRD_PARTIES 	= hal fatfs freertos
KERNEL_THIRD_PARTIES_LIBS = $(foreach lib,$(KERNEL_THIRD_PARTIES),-l$(lib)-$(BUILD_TYPE))

# FreeRTOS Kernel Directories
FREERTOS_DIR			= $(THIRD_PARTIES_DIR)/OS/FreeRTOS
FREERTOS_INCLUDES		= $(FREERTOS_DIR)/include
FREERTOS_SRCDIR			= $(FREERTOS_DIR)
FREERTOS_COMMON_DIR		= $(FREERTOS_DIR)/portable/Common
FREERTOS_MEMMANG_DIR	= $(FREERTOS_DIR)/portable/MemMang
FREERTOS_ARM_DIR		= $(FREERTOS_DIR)/portable/GCC/$(FREERTOS_PORTABLE)
FREERTOS_OBJDIR			= $(THIRD_PARTIES_OBJDIR)/freertos
FREERTOS_CONFDIR		= $(KERNEL_DIR)/conf

# FatFs Directories
FATFS_DIR		= $(THIRD_PARTIES_DIR)/FS/FatFs
FATFS_INCDIR	= $(FATFS_DIR)/source
FATFS_SRCDIR	= $(FATFS_DIR)/source
FATFS_OBJDIR	= $(THIRD_PARTIES_OBJDIR)/fatfs
FATFS_CONFDIR	= $(KERNEL_DIR)/conf

# HAL Directories
HALs_DIR		= $(THIRD_PARTIES_DIR)/HALs
HAL_DIR			= $(HALs_DIR)/HAL-$(CHIP_FAMILLY)
HAL_INCDIR		= $(HAL_DIR)/Inc
HAL_SRCDIR		= $(HAL_DIR)/Src
HAL_OBJDIR		= $(THIRD_PARTIES_OBJDIR)/hal
HAL_CONFDIR		= $(KERNEL_DIR)/conf

# CMSIS Directories
CMSIS_DIR 				= $(THIRD_PARTIES_DIR)/CMSIS
CMSIS_INCDIR 			= $(CMSIS_DIR)/CMSIS-ARM/CMSIS/Core/Include
CMSIS_INCDIR_DEVICE 	= $(CMSIS_DIR)/CMSIS-$(CHIP_FAMILLY)/Include

##############################################
################## KERNEL ####################
##############################################

# Internal path
KERNEL_OBJDIR = $(BUILD_DIR)/kernel

# Kernel Modules
KERNEL_MODULES = core drv fdir system

# Kernel files
KERNEL_SRCS = $(foreach m,$(KERNEL_MODULES), $(wildcard $(KERNEL_SRCDIR)/$(m)/*.c) $(wildcard $(KERNEL_SRCDIR)/$(m)/*/*.c) $(wildcard $(KERNEL_SRCDIR)/$(m)/*/wrapper-$(CHIP_VENDOR)/*.c)) \
			  $(wildcard $(KERNEL_SRCDIR)/bsp/$(BOARD)-BSP/src/*.c) \
			  $(SYS_CONF_SRCS) \
			  $(BSP_CONF_SRCS)
KERNEL_OBJS = $(patsubst $(KERNEL_SRCDIR)/%.c,$(KERNEL_OBJDIR)/%-$(BUILD_TYPE).o, \
			  $(patsubst $(PRE_BUILD_DIR)/conf/%.c,$(KERNEL_OBJDIR)/conf/%-$(BUILD_TYPE).o, \
			  $(KERNEL_SRCS)))
KERNEL_LIB  = $(LIBS_DIR)/libkernel-$(BUILD_TYPE).a

# System defines (those are use for system info const struct)
SYSTEM_DEFINES  = -DSYSTEM_NAME=\"TAPAS\"
SYSTEM_DEFINES += -DPROGRAM_NAME=\"$(PROJ_NAME)\"
SYSTEM_DEFINES += -DMAJOR=$(MAJOR)
SYSTEM_DEFINES += -DMINOR=$(MINOR)
SYSTEM_DEFINES += -DPATCH=$(PATCH)
SYSTEM_DEFINES += -DBUILD_TYPE=\"$(BUILD_TYPE)\"
SYSTEM_DEFINES += -DBOARD=\"$(BOARD)\"

# Kernel flags
KERNEL_CFLAGS    = $(PROJECT_CFLAGS)
KERNEL_INCFLAGS  = -I$(KERNEL_INCLUDES) -I$(KERNEL_INCDIR) -I$(KERNEL_INCDIR)/bsp/$(BOARD)-BSP/
KERNEL_INCFLAGS += -I$(FREERTOS_INCLUDES) -I$(FREERTOS_ARM_DIR) -I$(FREERTOS_CONFDIR)
KERNEL_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(HAL_CONFDIR)
KERNEL_INCFLAGS += -I$(FATFS_INCDIR) -I$(FATFS_CONFDIR)
KERNEL_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
KERNEL_INCFLAGS += -I$(PRE_BUILD_DIR)

# Include dependencies
-include $(KERNEL_OBJS:.o=.d)

# Kernel recipes
.PHONY : kernel kernel-start kernel-end kernel-clean
kernel: kernel-start $(KERNEL_LIB) kernel-end

# Build header
kernel-start :
	@echo "============================="
	@echo "===         KERNEL        ==="
	@echo "============================="
	@echo "Files to compile: $(words $(KERNEL_SRCS))"
	@echo "Compilation Flags:"
	@echo $(KERNEL_CFLAGS)
	@echo "Include Paths:"
	@echo $(KERNEL_INCFLAGS)
	@echo "Version Flags:"
	@echo $(VERSION_FLAGS)
	@echo "Start building:"

# Building recipes
$(KERNEL_OBJDIR)/%-$(BUILD_TYPE).o : $(KERNEL_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(KERNEL_CFLAGS) $(KERNEL_INCFLAGS) $(VERSION_FLAGS) $< -o $@

$(KERNEL_OBJDIR)/conf/%-$(BUILD_TYPE).o  : $(PRE_BUILD_DIR)/conf/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(KERNEL_CFLAGS) $(KERNEL_INCFLAGS) $(VERSION_FLAGS) $< -o $@

$(KERNEL_OBJDIR)/system/sysinfo-$(BUILD_TYPE).o : $(KERNEL_SRCDIR)/system/sysinfo.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(KERNEL_CFLAGS) $(SYSTEM_DEFINES) $(KERNEL_INCFLAGS) $(VERSION_FLAGS) $< -o $@

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


##############################################
#################### HAL #####################
##############################################

# HAL files
include $(HAL_CONFDIR)/HAL_SRCS_$(CHIP_FAMILLY).mk
HAL_OBJS  = $(subst $(HAL_SRCDIR)/,$(HAL_OBJDIR)/,$(HAL_SRCS:.c=-$(BUILD_TYPE).o))
HAL_LIB   = $(LIBS_DIR)/libhal-$(BUILD_TYPE).a

# HAL flags
HAL_CFLAGS    = $(PROJECT_CFLAGS) -Wno-unused-variable -Wno-unused-parameter
HAL_INCFLAGS  = -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(HAL_CONFDIR)
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
FATFS_INCFLAGS  = -I$(FATFS_INCDIR) -I$(FATFS_CONFDIR)
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
################## FREERTOS ##################
##############################################

# FREERTOS files
FREERTOS_SRCS = $(wildcard $(FREERTOS_SRCDIR)/*.c $(FREERTOS_ARM_DIR)/*.c $(FREERTOS_COMMON_DIR)/*.c)
FREERTOS_OBJS = $(subst $(FREERTOS_SRCDIR)/,$(FREERTOS_OBJDIR)/,$(FREERTOS_SRCS:.c=-$(BUILD_TYPE).o))
FREERTOS_LIB  = $(LIBS_DIR)/libfreertos-$(BUILD_TYPE).a

# FREERTOS flags
FREERTOS_CFLAGS    = $(PROJECT_CFLAGS) -Wno-unused-variable -Wno-unused-parameter -Wno-pedantic
FREERTOS_INCFLAGS  = -I$(FREERTOS_INCLUDES) -I$(FREERTOS_ARM_DIR) -I$(FREERTOS_CONFDIR)
FREERTOS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)
FREERTOS_INCFLAGS += -I$(PRE_BUILD_DIR)

# Include dependencies
-include $(FREERTOS_OBJS:.o=.d)

# FREERTOS recipes
.PHONY : freertos freertos-start freertos-end freertos-clean
freertos : freertos-start $(FREERTOS_LIB) freertos-end

# Build header
freertos-start :
	@echo "============================="
	@echo "===        FREERTOS       ==="
	@echo "============================="
	@echo "Files to compile: $(words $(FREERTOS_SRCS))"
	@echo "Compilation Flags:"
	@echo $(FREERTOS_CFLAGS)
	@echo "Include Paths:"
	@echo $(FREERTOS_INCFLAGS)
	@echo "Version Flags:"
	@echo $(VERSION_FLAGS)
	@echo "Start building:"

# Building recipes
$(FREERTOS_OBJDIR)/%-$(BUILD_TYPE).o : $(FREERTOS_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(FREERTOS_CFLAGS) $(FREERTOS_INCFLAGS) $(VERSION_FLAGS) $< -o $@

# Library generation
$(FREERTOS_LIB) : $(FREERTOS_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
freertos-end :
	@echo "Build done"
	@echo ""

# Clean recipe
freertos-clean :
	@echo "Cleaning FREERTOS build directory ..."
	@rm -rf $(FREERTOS_OBJDIR)
	@rm -rf $(FREERTOS_LIB)
	@echo "Done"

endif # BUILD_KERNEL_MK #