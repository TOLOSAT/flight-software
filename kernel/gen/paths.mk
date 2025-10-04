# Makefile with kernel paths

ifndef PATH_MK
PATH_MK := yes

##############################################
############# KERNEL DIRECTORIES #############
##############################################

# Depending if the workspace is provided by the top level or not
ifeq ($(origin WORKSPACE), undefined)
KERNEL_DIR 	= $(CURDIR)
BUILD_DIR 	= $(KERNEL_DIR)/build
else
KERNEL_DIR  = $(WORKSPACE)/kernel
BUILD_DIR   = $(WORKSPACE)/build
endif

KERNEL_HEADERS   		= $(KERNEL_DIR)/includes
BSP_DIR          		= $(KERNEL_DIR)/bsp/$(BOARD)-BSP
THIRD_PARTIES_DIR		= $(KERNEL_DIR)/third-parties
CONFIGS_DIR      		= $(KERNEL_DIR)/configs
TOOLS_DIR        		= $(KERNEL_DIR)/tools
GEN_DIR          		= $(KERNEL_DIR)/gen
THIRD_PARTIES_CONFDIR	= $(GEN_DIR)/conf
LIBS_DIR				= $(BUILD_DIR)/libs

# CMSIS Directories
CMSIS_DIR				= $(THIRD_PARTIES_DIR)/CMSIS
CMSIS_INCDIR			= $(CMSIS_DIR)/CMSIS-ARM/CMSIS/Core/Include
CMSIS_INCDIR_DEVICE 	= $(CMSIS_DIR)/CMSIS-$(CHIP_FAMILLY)/Include

endif # PATH_MK #