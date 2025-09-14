# Makefile with all project paths

ifndef PATH_MK
PATH_MK := yes

##############################################
############ PROJECT DIRECTORIES #############
##############################################

WORKSPACE			= .
CORE_DIR        	= $(WORKSPACE)/core
CONFIGS_DIR			= $(WORKSPACE)/configs
BSPs_DIR			= $(WORKSPACE)/bsp
THIRD_PARTIES_DIR	= $(WORKSPACE)/../third-parties
TOOLS_DIR			= $(WORKSPACE)/tools
GEN_DIR				= $(WORKSPACE)/gen
BUILD_DIR			= $(WORKSPACE)/build
LIBS_DIR			= $(BUILD_DIR)/libs
OUTPUT_DIR			= $(BUILD_DIR)/output

##############################################
########## CONFIGURATION DIRECTORIES #########
##############################################

CONF_FATFS_DIR		= $(GEN_DIR)/conf_FatFs
CONF_USBOTG_DIR		= $(GEN_DIR)/conf_USBOTG
CONF_HALS_DIR		= $(GEN_DIR)/conf_HALs
CONF_MISRA_DIR		= $(GEN_DIR)/conf_MISRA

##############################################
############ PRE-BUILD DIRECTORIES ###########
##############################################

PRE_BUILD_SCRIPTS_DIR	= $(TOOLS_DIR)
PRE_BUILD_DIR			= $(BUILD_DIR)/pre-build

##############################################
############## CORE DIRECTORIES ##############
##############################################

# Main Directories
CORE_INCDIR	= $(CORE_DIR)/inc
CORE_SRCDIR	= $(CORE_DIR)/src
CORE_OBJDIR	= $(BUILD_DIR)/core

##############################################
############### BSP DIRECTORIES ##############
##############################################

# BSP Directories
BSP_DIR		= $(BSPs_DIR)/$(BOARD)-BSP
BSP_INCDIR	= $(BSP_DIR)/inc
BSP_SRCDIR	= $(BSP_DIR)/src
BSP_LDDIR	= $(BSP_DIR)/ld
BSP_OBJDIR	= $(BUILD_DIR)/bsp

##############################################
########## THIRD PARTIES DIRECTORIES #########
##############################################

THIRD_PARTIES_OBJDIR	= $(BUILD_DIR)/third-parties

# FatFs Directories
FATFS_DIR		= $(THIRD_PARTIES_DIR)/FS/FatFs
FATFS_INCDIR	= $(FATFS_DIR)/source
FATFS_SRCDIR	= $(FATFS_DIR)/source
FATFS_OBJDIR	= $(THIRD_PARTIES_OBJDIR)/fatfs

# USBOTG Directories
USBOTG_DIR			= $(THIRD_PARTIES_DIR)/USBOTG/USBOTG-STM32-USB-Device
USBOTG_CORE_INCDIR	= $(USBOTG_DIR)/Core/Inc
USBOTG_MSC_INCDIR	= $(USBOTG_DIR)/Class/MSC/Inc
USBOTG_CORE_SRCDIR	= $(USBOTG_DIR)/Core/Src
USBOTG_MSC_SRCDIR	= $(USBOTG_DIR)/Class/MSC/Src
USBOTG_OBJDIR		= $(THIRD_PARTIES_OBJDIR)/usbotg

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
LD_SCRIPT		= $(BUILD_DIR)/$(PROJ_NAME).ld
KCONF_SCRIPT	= $(GEN_DIR)/Kconfig
DEFCONFIG_FILE	= $(CONFIGS_DIR)/$(CONFIG_NAME)_defconfig
RAW_LD_SCRIPT	= $(BSP_LDDIR)/$(LOAD_MEMORY).ld.S

endif # PATH_MK #