# Makefile with all project paths

ifndef PATH_MK
PATH_MK := yes

##############################################
############# KERNEL DIRECTORIES #############
##############################################

WORKSPACE			= $(CURDIR)
KERNEL_HEADERS		= $(WORKSPACE)/includes
BSP_DIR				= $(WORKSPACE)/bsp/$(BOARD)-BSP
THIRD_PARTIES_DIR	= $(WORKSPACE)/third-parties
CONFIGS_DIR			= $(WORKSPACE)/configs
TOOLS_DIR			= $(WORKSPACE)/tools
GEN_DIR				= $(WORKSPACE)/gen
BUILD_DIR			?= $(WORKSPACE)/build
LIBS_DIR			= $(BUILD_DIR)/libs

endif # PATH_MK #