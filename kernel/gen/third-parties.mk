# Makefile including all build recipes for kernel third parties

ifndef BUILD_THIRD_PARTIES_MK
BUILD_THIRD_PARTIES_MK := yes

##############################################
############# DIRECTORIES & FILES ############
##############################################

# Third Parties global dir
THIRD_PARTIES_CONFDIR	= $(GEN_DIR)/conf
LIBDIR 					= $(BUILD_DIR)/libs

# FreeRTOS Kernel Directories
FREERTOS_DIR			= $(THIRD_PARTIES_DIR)/OS/FreeRTOS
FREERTOS_INCLUDES		= $(FREERTOS_DIR)/include
FREERTOS_SRCDIR			= $(FREERTOS_DIR)
FREERTOS_COMMON_DIR		= $(FREERTOS_DIR)/portable/Common
FREERTOS_MEMMANG_DIR	= $(FREERTOS_DIR)/portable/MemMang
FREERTOS_ARM_DIR		= $(FREERTOS_DIR)/portable/GCC/$(FREERTOS_PORTABLE)
FREERTOS_OBJDIR			= $(BUILD_DIR)/third-parties/freertos

# FatFs Directories
FATFS_DIR		= $(THIRD_PARTIES_DIR)/FS/FatFs
FATFS_INCDIR	= $(FATFS_DIR)/source
FATFS_SRCDIR	= $(FATFS_DIR)/source
FATFS_OBJDIR	= $(BUILD_DIR)/third-parties/fatfs

# HAL Directories
HALs_DIR		= $(THIRD_PARTIES_DIR)/HALs
HAL_DIR			= $(HALs_DIR)/HAL-$(CHIP_FAMILLY)
HAL_INCDIR		= $(HAL_DIR)/Inc
HAL_SRCDIR		= $(HAL_DIR)/Src
HAL_OBJDIR		= $(BUILD_DIR)/third-parties/hal

# CMSIS Directories
CMSIS_DIR 				= $(THIRD_PARTIES_DIR)/CMSIS
CMSIS_INCDIR 			= $(CMSIS_DIR)/CMSIS-ARM/CMSIS/Core/Include
CMSIS_INCDIR_DEVICE 	= $(CMSIS_DIR)/CMSIS-$(CHIP_FAMILLY)/Include

##############################################
#################### HAL #####################
##############################################

# HAL files
include $(THIRD_PARTIES_CONFDIR)/HAL_SRCS_$(CHIP_FAMILLY).mk
HAL_OBJS  = $(subst $(HAL_SRCDIR)/,$(HAL_OBJDIR)/,$(HAL_SRCS:.c=.o))
HAL_LIB   = $(LIBDIR)/libhal.a

# HAL flags
HAL_CFLAGS    = $(CFLAGS) -Wno-unused-variable -Wno-unused-parameter
HAL_INCFLAGS  = -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(THIRD_PARTIES_CONFDIR)
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
	@echo "Start building:"

# Building recipes
$(HAL_OBJDIR)/%.o : $(HAL_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(HAL_CFLAGS) $(HAL_INCFLAGS) $< -o $@

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
FATFS_OBJS  = $(subst $(FATFS_SRCDIR)/,$(FATFS_OBJDIR)/,$(FATFS_SRCS:.c=.o))
FATFS_LIB   = $(LIBDIR)/libfatfs.a

# FATFS flags
FATFS_CFLAGS    = $(CFLAGS) -Wno-unused-variable -Wno-unused-parameter -Wno-stringop-overflow -Wno-unused-function
FATFS_INCFLAGS  = -I$(FATFS_INCDIR) -I$(THIRD_PARTIES_CONFDIR)
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
	@echo "Start building:"

# Building recipes
$(FATFS_OBJDIR)/%.o : $(FATFS_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(FATFS_CFLAGS) $(FATFS_INCFLAGS) $< -o $@

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
FREERTOS_SRCS = $(wildcard $(FREERTOS_SRCDIR)/*.c $(FREERTOS_ARM_DIR)/*.c $(FREERTOS_COMMON_DIR)/*.c $(FREERTOS_MEMMANG_DIR)/heap_1.c)
FREERTOS_OBJS = $(subst $(FREERTOS_SRCDIR)/,$(FREERTOS_OBJDIR)/,$(FREERTOS_SRCS:.c=.o))
FREERTOS_LIB  = $(LIBDIR)/libfreertos.a

# FREERTOS flags
FREERTOS_CFLAGS    = $(CFLAGS) -Wno-unused-variable -Wno-unused-parameter -Wno-pedantic
FREERTOS_INCFLAGS  = -I$(FREERTOS_INCLUDES) -I$(FREERTOS_ARM_DIR) -I$(THIRD_PARTIES_CONFDIR)
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
	@echo "Start building:"

# Building recipes
$(FREERTOS_OBJDIR)/%.o : $(FREERTOS_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(FREERTOS_CFLAGS) $(FREERTOS_INCFLAGS) $< -o $@

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

endif # BUILD_THIRD_PARTIES_MK #