# Makefile incluant tous les chemins

##############################################
################ DIRECTORIES #################
##############################################

# Directories
WORKSPACE = $(shell pwd)

CORE_DIR = $(WORKSPACE)/core
MAIN_DIR = $(CORE_DIR)/main

TOOLS_DIR = $(WORKSPACE)/tools
CMSIS_DIR = $(TOOLS_DIR)/CMSIS
HALs_DIR = $(TOOLS_DIR)/HALs
BSPs_DIR = $(TOOLS_DIR)/BSPs

CONF_DIR = $(WORKSPACE)/conf
LINKER_SCRIPTS_DIR = $(CONF_DIR)/linkerscripts

BUILD_DIR = $(WORKSPACE)/build
BUILD_TOOLS_DIR = $(BUILD_DIR)/tools
BUILD_CORE_DIR = $(BUILD_DIR)/core
BUILD_LIBS_DIR = $(BUILD_DIR)/libs
TARGET_DIR = $(BUILD_DIR)/target


