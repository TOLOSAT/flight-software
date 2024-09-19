# Makefile with all project paths

##############################################
############ PROJECT DIRECTORIES #############
##############################################

WORKSPACE 				= .

CORE_DIR        		= $(WORKSPACE)/core

BSPs_DIR  				= $(WORKSPACE)/../bsp
BSP_DIR 				= $(BSPs_DIR)/$(BOARD)-BSP

TOOLS_DIR 				= $(WORKSPACE)/../tools
CMSIS_DIR 				= $(TOOLS_DIR)/CMSIS
FATFS_DIR 				= $(TOOLS_DIR)/FS/FatFs
HALs_DIR  				= $(TOOLS_DIR)/HALs

GEN_DIR           		= $(WORKSPACE)/gen
CONF_BOARDS_DIR			= $(GEN_DIR)/conf_boards
CONF_FATFS_DIR     		= $(GEN_DIR)/conf_FatFs
CONF_HALS_DIR      		= $(GEN_DIR)/conf_HALs
CONF_MISRA_DIR     		= $(GEN_DIR)/conf_MISRA

BUILD_DIR             	= $(WORKSPACE)/build
BUILD_CORE_DIR        	= $(BUILD_DIR)/core
BUILD_BSP_DIR			= $(BUILD_DIR)/bsp
BUILD_TOOLS_DIR       	= $(BUILD_DIR)/tools
BUILD_LIBS_DIR        	= $(BUILD_DIR)/libs
TARGET_DIR            	= $(BUILD_DIR)/target

LD_SCRIPT				= $(BUILD_DIR)/$(PROJ_NAME).ld

##############################################
############## CORE DIRECTORIES ##############
##############################################

# Main Directories
CORE_INCDIR = $(CORE_DIR)/inc
CORE_SRCDIR = $(CORE_DIR)/src

##############################################
############## FATFS DIRECTORIES #############
##############################################

# FatFs Directories
FATFS_INCDIR = $(FATFS_DIR)/source
FATFS_SRCDIR = $(FATFS_DIR)/source
FATFS_OBJDIR = $(BUILD_DIR)/tools/fatfs

##############################################
############## CMSIS DIRECTORIES #############
##############################################

# CMSIS Directories
ifeq ($(CHIP_FAMILLY), STM32F4xx)
CMSIS_INCDIR_DEVICE = $(CMSIS_DIR)/cmsis_device_stm32f4/Include
else ifeq ($(CHIP_FAMILLY), STM32H7xx)
CMSIS_INCDIR_DEVICE = $(CMSIS_DIR)/cmsis_device_stm32h7/Include
else
$(error There is no compatible CMSIS)
endif
CMSIS_INCDIR       = $(CMSIS_DIR)/CMSIS-ARM/CMSIS/Core/Include

##############################################
############### HAL DIRECTORIES ##############
##############################################

# HAL Directories
ifeq ($(CHIP_FAMILLY), STM32F4xx)
HAL_DIR = $(HALs_DIR)/HAL-STM32F4
else ifeq ($(CHIP_FAMILLY), STM32H7xx)
HAL_DIR = $(HALs_DIR)/HAL-STM32H7
else
$(error There is no compatible HAL)
endif
HAL_INCDIR = $(HAL_DIR)/Inc
HAL_SRCDIR = $(HAL_DIR)/Src
HAL_OBJDIR = $(BUILD_TOOLS_DIR)/hal

##############################################
############### BSP DIRECTORIES ##############
##############################################

# BSP Directories
BSP_INCDIR = $(BSP_DIR)/inc
BSP_SRCDIR = $(BSP_DIR)/src
