# Makefile with all project paths

ifndef PATH_MK
PATH_MK := yes

##############################################
############ PROJECT DIRECTORIES #############
##############################################

WORKSPACE			= .
KERNEL_DIR			= $(WORKSPACE)/kernel
KERNEL_INCLUDES		= $(KERNEL_DIR)/includes
APPLICATIONS_DIR   ?= $(WORKSPACE)/applications
CONFIGS_DIR			= $(WORKSPACE)/configs
MIDDLEWARES_DIR		= $(WORKSPACE)/middlewares
TOOLS_DIR			= $(WORKSPACE)/tools
GEN_DIR				= $(WORKSPACE)/gen
TESTS_DIR			= $(WORKSPACE)/tests
BUILD_DIR			= $(WORKSPACE)/build
LIBS_DIR			= $(BUILD_DIR)/libs
OUTPUT_DIR			= $(BUILD_DIR)/output

##############################################
################ SPECIAL FILES ###############
##############################################

TARGET			= $(OUTPUT_DIR)/$(PROJ_NAME)-$(BUILD_TYPE).elf
KCONF_SCRIPT	= $(KERNEL_DIR)/Kconfig
DEFCONFIG_FILE	= $(CONFIGS_DIR)/$(CONFIG_NAME)_defconfig
LD_SCRIPT		= $(BUILD_DIR)/$(PROJ_NAME).ld

endif # PATH_MK #