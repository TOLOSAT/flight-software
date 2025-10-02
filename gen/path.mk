# Makefile with all project paths

ifndef PATH_MK
PATH_MK := yes

##############################################
############ PROJECT DIRECTORIES #############
##############################################

WORKSPACE			= $(CURDIR)
KERNEL_DIR			= $(WORKSPACE)/kernel
KERNEL_HEADERS		= $(KERNEL_DIR)/includes
APPLICATIONS_DIR   ?= $(WORKSPACE)/applications
CONFIGS_DIR			= $(WORKSPACE)/configs
MIDDLEWARES_DIR		= $(WORKSPACE)/middlewares
TOOLS_DIR			= $(WORKSPACE)/tools
GEN_DIR				= $(WORKSPACE)/gen
TESTS_DIR			= $(WORKSPACE)/tests
BUILD_DIR			= $(WORKSPACE)/build
LIBS_DIR			= $(BUILD_DIR)/libs
OUTPUT_DIR			= $(BUILD_DIR)/output
PRE_BUILD_DIR		= $(BUILD_DIR)/pre-build

##############################################
################ SPECIAL FILES ###############
##############################################

TARGET			= $(OUTPUT_DIR)/$(PROJ_NAME).elf
LD_SCRIPT		= $(KERNEL_DIR)/bsp/$(BOARD)-BSP/image.ld

endif # PATH_MK #