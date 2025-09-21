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
THIRD_PARTIES_DIR	= $(WORKSPACE)/third-parties
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

CONF_FATFS_DIR		= $(GEN_DIR)/conf_FatFs
CONF_FREERTOS_DIR	= $(GEN_DIR)/conf_FreeRTOS
CONF_HALS_DIR		= $(GEN_DIR)/conf_HALs
CONF_MISRA_DIR		= $(GEN_DIR)/conf_MISRA

##############################################
############ PRE-BUILD DIRECTORIES ###########
##############################################

PRE_BUILD_SCRIPTS_DIR	= $(TOOLS_DIR)
PRE_BUILD_DIR			= $(BUILD_DIR)/pre-build

##############################################
########## APPLICATIONS DIRECTORIES ##########
##############################################

# Applications Directories
APPLICATIONS_CONF_DIR	= $(APPLICATIONS_DIR)/conf
APPLICATIONS_OBJDIR		= $(BUILD_DIR)/applications

##############################################
########### MIDDLEWARES DIRECTORIES ###########
##############################################

MIDDLEWARES_OBJDIR	= $(BUILD_DIR)/middlewares

# PUS LIBRARY Directories
PUS_DIR		= $(MIDDLEWARES_DIR)/pus-library
PUS_INCDIR	= $(PUS_DIR)/inc
PUS_SRCDIR	= $(PUS_DIR)/src
PUS_OBJDIR	= $(MIDDLEWARES_OBJDIR)/pus

# IRIDIUM_DRV Directories
IRIDIUM_DIR		= $(MIDDLEWARES_DIR)/iridium-library
IRIDIUM_INCDIR	= $(IRIDIUM_DIR)/inc
IRIDIUM_SRCDIR	= $(IRIDIUM_DIR)/src
IRIDIUM_OBJDIR	= $(MIDDLEWARES_OBJDIR)/iridium

##############################################
########## THIRD PARTIES DIRECTORIES #########
##############################################

THIRD_PARTIES_OBJDIR	= $(BUILD_DIR)/third-parties

# FreeRTOS Kernel Directories
OS_DIR					= $(THIRD_PARTIES_DIR)/OS/FreeRTOS
OS_KERNEL_INCLUDES		= $(OS_DIR)/include
OS_KERNEL_SRCDIR		= $(OS_DIR)
OS_KERNEL_COMMON_DIR	= $(OS_DIR)/portable/Common
OS_KERNEL_MEMMANG_DIR	= $(OS_DIR)/portable/MemMang
OS_KERNEL_ARM_DIR		= $(OS_DIR)/portable/GCC/$(FREERTOS_PORTABLE)
OS_KERNEL_OBJDIR		= $(THIRD_PARTIES_OBJDIR)/os

# FatFs Directories
FATFS_DIR		= $(THIRD_PARTIES_DIR)/FS/FatFs
FATFS_INCDIR	= $(FATFS_DIR)/source
FATFS_SRCDIR	= $(FATFS_DIR)/source
FATFS_OBJDIR	= $(THIRD_PARTIES_OBJDIR)/fatfs

# HAL Directories
HALs_DIR	= $(THIRD_PARTIES_DIR)/HALs
HAL_DIR		= $(HALs_DIR)/HAL-$(CHIP_FAMILLY)
HAL_INCDIR	= $(HAL_DIR)/Inc
HAL_SRCDIR	= $(HAL_DIR)/Src
HAL_OBJDIR	= $(THIRD_PARTIES_OBJDIR)/hal

# CMSIS Directories
CMSIS_DIR 				= $(THIRD_PARTIES_DIR)/CMSIS
CMSIS_INCDIR 			= $(CMSIS_DIR)/CMSIS-ARM/CMSIS/Core/Include
CMSIS_INCDIR_DEVICE 	= $(CMSIS_DIR)/CMSIS-$(CHIP_FAMILLY)/Include

##############################################
################ SPECIAL FILES ###############
##############################################

TARGET			= $(OUTPUT_DIR)/$(PROJ_NAME)-$(BUILD_TYPE).elf
KCONF_SCRIPT	= $(GEN_DIR)/Kconfig
DEFCONFIG_FILE	= $(CONFIGS_DIR)/$(CONFIG_NAME)_defconfig
LD_SCRIPT		= $(BUILD_DIR)/$(PROJ_NAME).ld
RAW_LD_SCRIPT	= $(KERNEL_DIR)/bsp/$(BOARD)-BSP/$(LOAD_MEMORY).ld.S

endif # PATH_MK #