# Makefile incluant tous les chemins

##############################################
############ PROJECT DIRECTORIES #############
##############################################

WORKSPACE = $(shell pwd)

CORE_DIR = $(WORKSPACE)/core
MAIN_DIR = $(CORE_DIR)/main

TOOLS_DIR = $(WORKSPACE)/tools
CMSIS_DIR = $(TOOLS_DIR)/CMSIS
OS_DIR = $(TOOLS_DIR)/OS
HALs_DIR = $(TOOLS_DIR)/HALs
BSPs_DIR = $(TOOLS_DIR)/BSPs

CONF_DIR = $(WORKSPACE)/conf
LINKER_SCRIPTS_DIR = $(CONF_DIR)/linkerscripts

BUILD_DIR = $(WORKSPACE)/build
BUILD_TOOLS_DIR = $(BUILD_DIR)/tools
BUILD_CORE_DIR = $(BUILD_DIR)/core
BUILD_OS_DIR = $(BUILD_DIR)/os
BUILD_LIBS_DIR = $(BUILD_DIR)/libs
TARGET_DIR = $(BUILD_DIR)/target

##############################################
############## CORE DIRECTORIES ##############
##############################################

# Main Directories
MAIN_INCDIR = $(MAIN_DIR)/inc
MAIN_SRCDIR = $(MAIN_DIR)/src
MAIN_OBJDIR = $(BUILD_CORE_DIR)/main

##############################################
################ OS DIRECTORIES ##############
##############################################

# FreeRTOS Directories
OS_INCDIR = $(OS_DIR)/FreeRTOS-Kernel/include
OS_SRCDIR = $(OS_DIR)/FreeRTOS-Kernel
OS_MEMMANG_DIR = $(OS_DIR)/FreeRTOS-Kernel/portable/MemMang
OS_ARM_DIR = $(OS_DIR)/FreeRTOS-Kernel/portable/GCC/ARM_CM4F
OS_OBJDIR = $(BUILD_OS_DIR)

##############################################
############## CMSIS DIRECTORIES #############
##############################################

# CMSIS Directories
ifeq ($(CHIP_FAMILLY), STM32F4xx)
	CMSIS_INCDIR_DEVICE = $(CMSIS_DIR)/cmsis_device_stm32f4/Include
else
	$(error There is no compatible CMSIS)
endif
CMSIS_INCDIR = $(CMSIS_DIR)/CMSIS_ARM/CMSIS/Include

##############################################
############### HAL DIRECTORIES ##############
##############################################

# HAL Directories
ifeq ($(CHIP_FAMILLY), STM32F4xx)
	HAL_DIR = $(HALs_DIR)/HAL-STM32F4
else
	$(error There is no compatible HAL)
endif
HAL_INCDIR = $(HAL_DIR)/Inc
HAL_SRCDIR = $(HAL_DIR)/Src
HAL_OBJDIR = $(BUILD_TOOLS_DIR)/hal

##############################################
########### HAL TOLOSAT DIRECTORIES ##########
##############################################

# HAL TOLOSAT Directories
HAL_TOLOSAT_DIR = $(HALs_DIR)/HAL-TOLOSAT
HAL_TOLOSAT_INCDIR = $(HAL_TOLOSAT_DIR)/inc
HAL_TOLOSAT_SRCDIR = $(HAL_TOLOSAT_DIR)/src
HAL_TOLOSAT_OBJDIR = $(BUILD_TOOLS_DIR)/hal_tolosat

##############################################
############### BSP DIRECTORIES ##############
##############################################

# BSP Directories
ifeq ($(BOARD), NUCLEO-F411RE)
	BSP_DIR = $(BSPs_DIR)/STM32F411RE-bsp
else
	print := $(error This board is not available for the flight software)
endif
BSP_INCDIR = $(BSP_DIR)/inc
BSP_SRCDIR = $(BSP_DIR)/src
BSP_OBJDIR = $(BUILD_TOOLS_DIR)/bsp