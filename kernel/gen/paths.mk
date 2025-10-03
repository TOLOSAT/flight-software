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

KERNEL_HEADERS   	= $(KERNEL_DIR)/includes
BSP_DIR          	= $(KERNEL_DIR)/bsp/$(BOARD)-BSP
THIRD_PARTIES_DIR	= $(KERNEL_DIR)/third-parties
CONFIGS_DIR      	= $(KERNEL_DIR)/configs
TOOLS_DIR        	= $(KERNEL_DIR)/tools
GEN_DIR          	= $(KERNEL_DIR)/gen
LIBS_DIR         	= $(BUILD_DIR)/libs

endif # PATH_MK #