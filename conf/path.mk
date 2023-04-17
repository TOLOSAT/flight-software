# Makefile incluant tous les chemins

##############################################
############ PROJECT DIRECTORIES #############
##############################################

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

##############################################
############## CORE DIRECTORIES ##############
##############################################

# Main Directories
MAIN_INCDIR = $(MAIN_DIR)/inc
MAIN_SRCDIR = $(MAIN_DIR)/src
MAIN_OBJDIR = $(BUILD_CORE_DIR)/main

# Files
MAIN_SRCS = $(wildcard $(MAIN_SRCDIR)/*.c)
MAIN_OBJS = $(MAIN_SRCS:.c=.o)
MAIN_OBJS := $(subst $(MAIN_SRCDIR)/,$(MAIN_OBJDIR)/,$(MAIN_OBJS))

##############################################
############## CMSIS DIRECTORIES #############
##############################################

# CMSIS Directories
CMSIS_INCDIR = $(CMSIS_DIR)/Include
CMSIS_INCDIR_DEVICE = $(CMSIS_DIR)/Device/$(CHIP_VENDOR)/$(CHIP_FAMILLY)/Include
CMSIS_SRCDIR_DEVICE = $(CMSIS_DIR)/Device/$(CHIP_VENDOR)/$(CHIP_FAMILLY)/Source
CMSIS_OBJDIR = $(BUILD_TOOLS_DIR)/cmsis

# CMSIS Files
CMSIS_SRCS = $(wildcard $(CMSIS_SRCDIR_DEVICE)/*.c)
CMSIS_OBJS = $(CMSIS_SRCS:.c=.o)
CMSIS_OBJS := $(subst $(CMSIS_SRCDIR_DEVICE)/,$(CMSIS_OBJDIR)/,$(CMSIS_OBJS))
CMSIS_LIB = $(BUILD_LIBS_DIR)/libcmsis.a

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

# HAL Files
HAL_SRCS = $(wildcard $(HAL_SRCDIR)/*.c $(HAL_SRCDIR)/Legacy/*.c)
HAL_OBJS = $(HAL_SRCS:.c=.o)
HAL_OBJS := $(subst $(HAL_SRCDIR)/,$(HAL_OBJDIR)/,$(HAL_OBJS))
HAL_LIB = $(BUILD_LIBS_DIR)/libhal.a

##############################################
########### HAL TOLOSAT DIRECTORIES ##########
##############################################

# HAL Directories
HAL_TOLOSAT_DIR = $(HALs_DIR)/HAL-TOLOSAT
HAL_TOLOSAT_INCDIR = $(HAL_TOLOSAT_DIR)/inc
HAL_TOLOSAT_SRCDIR = $(HAL_TOLOSAT_DIR)/src
HAL_TOLOSAT_OBJDIR = $(BUILD_TOOLS_DIR)/hal_tolosat

# HAL Files
HAL_TOLOSAT_SRCS = $(wildcard $(HAL_TOLOSAT_SRCDIR)/*.c)
HAL_TOLOSAT_OBJS = $(HAL_TOLOSAT_SRCS:.c=.o)
HAL_TOLOSAT_OBJS := $(subst $(HAL_TOLOSAT_SRCDIR)/,$(HAL_TOLOSAT_OBJDIR)/,$(HAL_TOLOSAT_OBJS))
HAL_TOLOSAT_LIB = $(BUILD_LIBS_DIR)/libhal-tolosat.a

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

# Files
BSP_SRCS = $(wildcard $(BSP_SRCDIR)/*.c)
BSP_OBJS = $(BSP_SRCS:.c=.o)
BSP_OBJS := $(subst $(BSP_SRCDIR)/,$(BSP_OBJDIR)/,$(BSP_OBJS))