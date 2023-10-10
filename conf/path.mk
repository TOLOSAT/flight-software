# Makefile with all project paths

##############################################
############ PROJECT DIRECTORIES #############
##############################################

WORKSPACE = $(shell pwd)

CORE_DIR        = $(WORKSPACE)/core

MIDDLEWARES_DIR = $(WORKSPACE)/middlewares
LIBPUS_DIR      = $(MIDDLEWARES_DIR)/libpus
TOLOSAT_FS_DIR  = $(MIDDLEWARES_DIR)/file-system

TOOLS_DIR = $(WORKSPACE)/tools
CMSIS_DIR = $(TOOLS_DIR)/CMSIS
OS_DIR    = $(TOOLS_DIR)/OS/FreeRTOS
FATFS_DIR = $(TOOLS_DIR)/FS/FatFs
HALs_DIR  = $(TOOLS_DIR)/HALs
BSPs_DIR  = $(TOOLS_DIR)/BSPs

CONF_DIR           = $(WORKSPACE)/conf
LINKER_SCRIPTS_DIR = $(CONF_DIR)/linkerscripts
CONF_FREERTOS_DIR  = $(CONF_DIR)/conf_FreeRTOS
CONF_FATFS_DIR      = $(CONF_DIR)/conf_FatFs
CONF_HALS_DIR      = $(CONF_DIR)/conf_HALs
CONF_MISRA_DIR     = $(CONF_DIR)/conf_misra

TESTS_DIR = $(WORKSPACE)/tests

ifeq ($(TEST),)
ifeq ($(findstring test-,$(MAKECMDGOALS)),test-)
TEST_NAME := $(subst test-,test-,$(filter test-%,$(MAKECMDGOALS)))
APPLICATION_DIR = $(TESTS_DIR)/$(TEST_NAME)
else
APPLICATION_DIR = $(WORKSPACE)/application
endif
else
APPLICATION_DIR = $(TESTS_DIR)/$(TEST)
endif

BUILD_DIR             = $(WORKSPACE)/build
BUILD_CORE_DIR        = $(BUILD_DIR)/core
BUILD_APPLICATION_DIR = $(BUILD_DIR)/application
BUILD_MIDDLEWARES_DIR = $(BUILD_DIR)/middlewares
BUILD_TOOLS_DIR       = $(BUILD_DIR)/tools
BUILD_OS_DIR          = $(BUILD_DIR)/tools/os
BUILD_FATFS_DIR       = $(BUILD_DIR)/tools/fatfs
BUILD_LIBS_DIR        = $(BUILD_DIR)/libs
TARGET_DIR            = $(BUILD_DIR)/target

##############################################
############## CORE DIRECTORIES ##############
##############################################

# Main Directories
CORE_INCDIR = $(CORE_DIR)/inc
CORE_SRCDIR = $(CORE_DIR)/src

##############################################
########### APPLICATION DIRECTORIES ##########
##############################################

# Application Directories
APPLICATION_INCDIR = $(APPLICATION_DIR)/inc
APPLICATION_SRCDIR = $(APPLICATION_DIR)/src

##############################################
################ OS DIRECTORIES ##############
##############################################²

# FreeRTOS Kernel Directories
OS_KERNEL_INCDIR      = $(OS_DIR)/Source/include
OS_KERNEL_SRCDIR      = $(OS_DIR)/Source
OS_KERNEL_MEMMANG_DIR = $(OS_DIR)/Source/portable/MemMang
ifeq ($(CHIP_FAMILLY), STM32F4xx)
OS_KERNEL_ARM_DIR     = $(OS_DIR)/Source/portable/GCC/ARM_CM4F
else ifeq ($(CHIP_FAMILLY), STM32F1xx)
OS_KERNEL_ARM_DIR     = $(OS_DIR)/Source/portable/GCC/ARM_CM3
else ifeq ($(CHIP_FAMILLY), STM32H7xx)
OS_KERNEL_ARM_DIR     = $(OS_DIR)/Source/portable/GCC/ARM_CM7/r0p1
else
$(error There is no compatible OS)
endif
OS_CMSIS_RTOSV2_DIR   = $(OS_DIR)/CMSIS/RTOS2/FreeRTOS
OS_KERNEL_OBJDIR      = $(BUILD_OS_DIR)

# RTOS2 CMSIS Directories
OS_CMSIS_SRCDIR = $(OS_CMSIS_RTOSV2_DIR)/Source
OS_CMSIS_OBJDIR = $(BUILD_OS_DIR)/cmsis

##############################################
########### MIDDLEWARES DIRECTORIES ###########
##############################################

# LIBPUS Directories
LIBPUS_INCDIR = $(LIBPUS_DIR)/inc
LIBPUS_SRCDIR = $(LIBPUS_DIR)/src
LIBPUS_OBJDIR = $(BUILD_MIDDLEWARES_DIR)/libpus

# TOLOSAT_FS Directories
TOLOSAT_FS_INCDIR = $(TOLOSAT_FS_DIR)/inc
TOLOSAT_FS_SRCDIR = $(TOLOSAT_FS_DIR)/src
TOLOSAT_FS_OBJDIR = $(BUILD_MIDDLEWARES_DIR)/libtolosat-fs

##############################################
############## FATFS DIRECTORIES #############
##############################################

# FatFs Directories
FATFS_INCDIR = $(FATFS_DIR)/src
FATFS_SRCDIR = $(FATFS_DIR)/src
FATFS_OBJDIR = $(BUILD_FATFS_DIR)

##############################################
############## CMSIS DIRECTORIES #############
##############################################

# CMSIS Directories
ifeq ($(CHIP_FAMILLY), STM32F4xx)
CMSIS_INCDIR_DEVICE = $(CMSIS_DIR)/cmsis_device_stm32f4/Include
else ifeq ($(CHIP_FAMILLY), STM32F1xx)
CMSIS_INCDIR_DEVICE = $(CMSIS_DIR)/cmsis_device_stm32f1/Include
else ifeq ($(CHIP_FAMILLY), STM32H7xx)
CMSIS_INCDIR_DEVICE = $(CMSIS_DIR)/cmsis_device_stm32h7/Include
else
$(error There is no compatible CMSIS)
endif
CMSIS_INCDIR       = $(CMSIS_DIR)/CMSIS-ARM/CMSIS/Core/Include
CMSIS_RTOS2_INCDIR = $(CMSIS_DIR)/CMSIS-ARM/CMSIS/RTOS2/Include

##############################################
############### HAL DIRECTORIES ##############
##############################################

# HAL Directories
ifeq ($(CHIP_FAMILLY), STM32F4xx)
HAL_DIR = $(HALs_DIR)/HAL-STM32F4
else ifeq ($(CHIP_FAMILLY), STM32F1xx)
HAL_DIR = $(HALs_DIR)/HAL-STM32F1
else ifeq ($(CHIP_FAMILLY), STM32H7xx)
HAL_DIR = $(HALs_DIR)/HAL-STM32H7
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
HAL_TOLOSAT_DIR    = $(HALs_DIR)/HAL-TOLOSAT
HAL_TOLOSAT_INCDIR = $(HAL_TOLOSAT_DIR)/inc
HAL_TOLOSAT_SRCDIR = $(HAL_TOLOSAT_DIR)/src
HAL_TOLOSAT_OBJDIR = $(BUILD_TOOLS_DIR)/hal_tolosat

##############################################
############### BSP DIRECTORIES ##############
##############################################

# BSP Directories
BSP_INCDIR = $(BSP_DIR)/inc
BSP_SRCDIR = $(BSP_DIR)/src
BSP_OBJDIR = $(BUILD_TOOLS_DIR)/bsp
