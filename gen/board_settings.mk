# Makefile with board parameters

#################################
######### NUCLEO_H745ZI #########
#################################
ifeq ($(BOARD), NUCLEO_H745ZI)
# Chip Information
CHIP_VENDOR = ST
CHIP_FAMILLY = STM32H7xx
CHIP = STM32H745xx
MACH = cortex-m7
CORE_SELECT = -DCORE_CM7
FPU_SETTINGS = -DFPU_AVAILABLE -mfpu=fpv5-d16 -mfloat-abi=hard
CACHE_SETTINGS = -DCACHE_AVAILABLE
MPU_SETTINGS = -DMPU_AVAILABLE
FREERTOS_PORTABLE = ARM_CM4_MPU
# Debugger Information
OCD_DBG = interface/stlink.cfg
OCD_CHIP = target/stm32h7x.cfg
# HAL & BSP Information
HAL_SRCS_LIST = $(CONF_HALS_DIR)/stm32h7xx_hal_conf.mk
LINKER_SCRIPT = $(BSP_DIR)/stm32h745zi_mpu_$(LOAD_MEMORY).ld
BSP_DIR = $(BSPs_DIR)/NUCLEO-H745ZI-BSP

#################################
######### NUCLEO_F411RE #########
#################################
else ifeq ($(BOARD), NUCLEO_F411RE)
# Chip Information
CHIP_VENDOR = ST
CHIP_FAMILLY = STM32F4xx
CHIP = STM32F411xE
MACH = cortex-m4
CORE_SELECT =
FPU_SETTINGS = -DFPU_AVAILABLE -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CACHE_SETTINGS = -DCACHE_UNAVAILABLE
MPU_SETTINGS = -DMPU_UNAVAILABLE
FREERTOS_PORTABLE = ARM_CM4F
# Debugger Information
OCD_DBG = interface/stlink.cfg
OCD_CHIP = target/stm32f4x.cfg
# HAL & BSP Information
HAL_SRCS_LIST = $(CONF_HALS_DIR)/stm32f4xx_hal_conf.mk
BSP_DIR = $(BSPs_DIR)/NUCLEO-F411RE-BSP
LINKER_SCRIPT = $(BSP_DIR)/stm32f411re_$(LOAD_MEMORY).ld

#################################
######## DISCOVERY_F407VG #######
#################################
else ifeq ($(BOARD), DISCOVERY_F407VG)
# Chip Information
CHIP_VENDOR = ST
CHIP_FAMILLY = STM32F4xx
CHIP = STM32F407xx
MACH = cortex-m4
CORE_SELECT =
FPU_SETTINGS = -DFPU_AVAILABLE -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CACHE_SETTINGS = -DCACHE_UNAVAILABLE
MPU_SETTINGS = -DMPU_UNAVAILABLE
FREERTOS_PORTABLE = ARM_CM4F
# Debugger Information
OCD_DBG = interface/stlink.cfg
OCD_CHIP = target/stm32f4x.cfg
# HAL & BSP Information
HAL_SRCS_LIST = $(CONF_HALS_DIR)/stm32f4xx_hal_conf.mk
BSP_DIR = $(BSPs_DIR)/DISCOVERY-F407VG-BSP
LINKER_SCRIPT = $(BSP_DIR)/stm32f407vg_$(LOAD_MEMORY).ld

#################################
######### NUCLEO_F103RB #########
#################################
else ifeq ($(BOARD), NUCLEO_F103RB)
# Chip Information
CHIP_VENDOR = ST
CHIP_FAMILLY = STM32F1xx
CHIP = STM32F103xB
MACH = cortex-m3
CORE_SELECT =
FPU_SETTINGS = -DFPU_UNAVAILABLE -mfloat-abi=soft
CACHE_SETTINGS = -DCACHE_UNAVAILABLE
MPU_SETTINGS = -DMPU_UNAVAILABLE
FREERTOS_PORTABLE = ARM_CM3
# Debugger Information
OCD_DBG = interface/stlink.cfg
OCD_CHIP = target/stm32f1x.cfg
# HAL & BSP Information
HAL_SRCS_LIST = $(CONF_HALS_DIR)/stm32f1xx_hal_conf.mk
BSP_DIR = $(BSPs_DIR)/NUCLEO-F103RB-BSP
LINKER_SCRIPT = $(BSP_DIR)/stm32f103rb_$(LOAD_MEMORY).ld

#################################
############## ELSE #############
#################################
else
$(error This board is not available for the flight software)
endif