# Makefile with all project paths

ifndef PATH_MK
PATH_MK := yes

##############################################
############ PROJECT DIRECTORIES #############
##############################################

WORKSPACE			= .
CORE_DIR			= $(WORKSPACE)/core
APPLICATION_DIR	   ?= $(WORKSPACE)/application
CONFIGS_DIR			= $(WORKSPACE)/configs
MIDDLEWARES_DIR		= $(WORKSPACE)/middlewares
BSPs_DIR			= $(WORKSPACE)/bsp
TOOLS_DIR			= $(WORKSPACE)/tools
GEN_DIR				= $(WORKSPACE)/gen
TESTS_DIR			= $(WORKSPACE)/tests
BOOT_DIR			= $(WORKSPACE)/bootloader
BUILD_DIR			= $(WORKSPACE)/build
LIBS_DIR			= $(BUILD_DIR)/libs
OUTPUT_DIR			= $(BUILD_DIR)/output

##############################################
########## CONFIGURATION DIRECTORIES #########
##############################################

CONF_BOARDS_DIR		= $(GEN_DIR)/conf_boards
CONF_FATFS_DIR		= $(GEN_DIR)/conf_FatFs
CONF_FREERTOS_DIR	= $(GEN_DIR)/conf_FreeRTOS
CONF_HALS_DIR		= $(GEN_DIR)/conf_HALs
CONF_MISRA_DIR		= $(GEN_DIR)/conf_misra

##############################################
############ PRE-BUILD DIRECTORIES ###########
##############################################

PRE_BUILD_SCRIPTS_DIR	= $(TOOLS_DIR)/pre-build
PRE_BUILD_DIR			= $(BUILD_DIR)/pre-build

##############################################
############## CORE DIRECTORIES ##############
##############################################

# Core Directories
CORE_INCDIR	= $(CORE_DIR)/inc
CORE_SRCDIR	= $(CORE_DIR)/src
CORE_OBJDIR	= $(BUILD_DIR)/core

##############################################
########### APPLICATION DIRECTORIES ##########
##############################################

# Application Directories
APPLICATION_CONF_DIR	= $(APPLICATION_DIR)/conf
APPLICATION_INCDIR		= $(APPLICATION_DIR)/inc
APPLICATION_SRCDIR		= $(APPLICATION_DIR)/src
APPLICATION_OBJDIR		= $(BUILD_DIR)/application

##############################################
########### MIDDLEWARES DIRECTORIES ###########
##############################################

# LIBPUS Directories
PUS_DIR		= $(MIDDLEWARES_DIR)/libpus
PUS_INCDIR	= $(PUS_DIR)/inc
PUS_SRCDIR	= $(PUS_DIR)/src
PUS_OBJDIR	= $(BUILD_DIR)/libpus

# IRIDIUM_DRV Directories
IRIDIUMDRV_DIR		= $(MIDDLEWARES_DIR)/iridium-driver
IRIDIUMDRV_INCDIR	= $(IRIDIUMDRV_DIR)/inc
IRIDIUMDRV_SRCDIR	= $(IRIDIUMDRV_DIR)/src
IRIDIUMDRV_OBJDIR	= $(BUILD_DIR)/iridiumdrv

##############################################
############### BSP DIRECTORIES ##############
##############################################

# BSP Directories
BSP_DIR		= $(BSPs_DIR)/$(BOARD)-BSP
BSP_INCDIR	= $(BSP_DIR)/inc
BSP_SRCDIR	= $(BSP_DIR)/src
BSP_OBJDIR	= $(BUILD_DIR)/bsp

##############################################
########## THIRD PARTIES DIRECTORIES #########
##############################################

# FreeRTOS Kernel Directories
OS_DIR					= $(TOOLS_DIR)/OS/FreeRTOS
OS_KERNEL_INCDIR		= $(OS_DIR)/include
OS_KERNEL_SRCDIR		= $(OS_DIR)
OS_KERNEL_COMMON_DIR	= $(OS_DIR)/portable/Common
OS_KERNEL_MEMMANG_DIR	= $(OS_DIR)/portable/MemMang
OS_KERNEL_ARM_DIR		= $(OS_DIR)/portable/GCC/$(FREERTOS_PORTABLE)
OS_KERNEL_OBJDIR		= $(BUILD_DIR)/os

# FatFs Directories
FATFS_DIR		= $(TOOLS_DIR)/FS/FatFs
FATFS_INCDIR	= $(FATFS_DIR)/source
FATFS_SRCDIR	= $(FATFS_DIR)/source
FATFS_OBJDIR	= $(BUILD_DIR)/fatfs

# HAL Directories
HALs_DIR	= $(TOOLS_DIR)/HALs
HAL_DIR		= $(HALs_DIR)/HAL-$(CHIP_FAMILLY)
HAL_INCDIR	= $(HAL_DIR)/Inc
HAL_SRCDIR	= $(HAL_DIR)/Src
HAL_OBJDIR	= $(BUILD_DIR)/hal

# CMSIS Directories
CMSIS_DIR 				= $(TOOLS_DIR)/CMSIS
CMSIS_INCDIR 			= $(CMSIS_DIR)/CMSIS-ARM/CMSIS/Core/Include
CMSIS_INCDIR_DEVICE 	= $(CMSIS_DIR)/CMSIS-$(CHIP_FAMILLY)/Include

##############################################
################ SPECIAL FILES ###############
##############################################

TARGET			= $(OUTPUT_DIR)/$(PROJ_NAME)-$(BUILD_TYPE).elf
LD_SCRIPT		= $(BUILD_DIR)/$(PROJ_NAME).ld
KCONF_SCRIPT	= $(GEN_DIR)/Kconfig
DEFCONFIG_FILE	= $(CONFIGS_DIR)/$(CONFIG_NAME)_defconfig
RAW_LD_SCRIPT	= $(BSP_DIR)/$(BOARD)_$(LOAD_MEMORY).ldf

endif # PATH_MK #