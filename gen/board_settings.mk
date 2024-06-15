# Makefile with board parameters

#################################
############# ART_PI ############
#################################
ifeq ($(BOARD), ART_PI)
# Chip Information
CHIP_VENDOR = ST
CHIP_FAMILLY = STM32H7xx
CHIP = STM32H750xx
MACH = cortex-m7
CORE_SELECT = -DCORE_CM7
FPU_TYPE = -mfpu=fpv5-d16 -mfloat-abi=hard
FPU_AVAILABILITY = FPU_AVAILABLE
CACHE_AVAILABILITY = CACHE_AVAILABLE
MPU_AVAILABILITY = MPU_AVAILABLE
ECC_AVAILABILITY = ECC_UNAVAILABLE
FREERTOS_PORTABLE = ARM_CM4_MPU
# Debugger Information
OCD_DBG = interface/stlink.cfg
OCD_CHIP = target/stm32h7x.cfg
# HAL & BSP Information
HAL_SRCS_LIST = $(CONF_HALS_DIR)/stm32h7xx_hal_conf.mk
BSP_DIR = $(BSPs_DIR)/ART-PI-BSP
BSP_LD_SCRIPT = $(BSP_DIR)/stm32h750xb_$(shell echo $(LOAD_MEMORY) | tr '[:upper:]' '[:lower:]').ldf

#################################
############## QEMU #############
#################################
else ifeq ($(BOARD), QEMU)
# Chip Information
CHIP_VENDOR = ARM
CHIP_FAMILLY = CMSDK
CHIP = CMSDK_CM7
MACH = cortex-m7
CORE_SELECT = -DCORE_CM7
FPU_TYPE = -mfpu=fpv5-d16 -mfloat-abi=hard
FPU_AVAILABILITY = FPU_AVAILABLE
CACHE_AVAILABILITY = CACHE_AVAILABLE
MPU_AVAILABILITY = MPU_UNAVAILABLE
ECC_AVAILABILITY = ECC_UNAVAILABLE
FREERTOS_PORTABLE = ARM_CM4F
# Debugger Information
QEMU_MACHINE = mps2-an500
# HAL & BSP Information
HAL_SRCS_LIST = $(CONF_HALS_DIR)/cmsdk_hal_conf.mk
BSP_DIR = $(BSPs_DIR)/QEMU-BSP
BSP_LD_SCRIPT = $(BSP_DIR)/mps2_an500_$(shell echo $(LOAD_MEMORY) | tr '[:upper:]' '[:lower:]').ldf

#################################
######### NUCLEO_H745ZI #########
#################################
else ifeq ($(BOARD), NUCLEO_H745ZI)
# Chip Information
CHIP_VENDOR = ST
CHIP_FAMILLY = STM32H7xx
CHIP = STM32H745xx
MACH = cortex-m7
CORE_SELECT = -DCORE_CM7
FPU_TYPE = -mfpu=fpv5-d16 -mfloat-abi=hard
FPU_AVAILABILITY = FPU_AVAILABLE
CACHE_AVAILABILITY = CACHE_AVAILABLE
MPU_AVAILABILITY = MPU_AVAILABLE
ECC_AVAILABILITY = ECC_UNAVAILABLE
FREERTOS_PORTABLE = ARM_CM4_MPU
# Debugger Information
OCD_DBG = interface/stlink.cfg
OCD_CHIP = target/stm32h7x.cfg
# HAL & BSP Information
HAL_SRCS_LIST = $(CONF_HALS_DIR)/stm32h7xx_hal_conf.mk
BSP_DIR = $(BSPs_DIR)/NUCLEO-H745ZI-BSP
BSP_LD_SCRIPT = $(BSP_DIR)/stm32h745zi_$(shell echo $(LOAD_MEMORY) | tr '[:upper:]' '[:lower:]').ldf

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
FPU_TYPE = -mfpu=fpv4-sp-d16 -mfloat-abi=hard
FPU_AVAILABILITY = FPU_AVAILABLE
CACHE_AVAILABILITY = CACHE_UNAVAILABLE
MPU_AVAILABILITY = MPU_UNAVAILABLE
ECC_AVAILABILITY = ECC_UNAVAILABLE
FREERTOS_PORTABLE = ARM_CM4F
# Debugger Information
OCD_DBG = interface/stlink.cfg
OCD_CHIP = target/stm32f4x.cfg
# HAL & BSP Information
HAL_SRCS_LIST = $(CONF_HALS_DIR)/stm32f4xx_hal_conf.mk
BSP_DIR = $(BSPs_DIR)/NUCLEO-F411RE-BSP
BSP_LD_SCRIPT = $(BSP_DIR)/stm32f411re_$(shell echo $(LOAD_MEMORY) | tr '[:upper:]' '[:lower:]').ldf

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
FPU_TYPE = -mfpu=fpv4-sp-d16 -mfloat-abi=hard
FPU_AVAILABILITY = FPU_AVAILABLE
CACHE_AVAILABILITY = CACHE_UNAVAILABLE
MPU_AVAILABILITY = MPU_UNAVAILABLE
ECC_AVAILABILITY = ECC_UNAVAILABLE
FREERTOS_PORTABLE = ARM_CM4F
# Debugger Information
OCD_DBG = interface/stlink.cfg
OCD_CHIP = target/stm32f4x.cfg
# HAL & BSP Information
HAL_SRCS_LIST = $(CONF_HALS_DIR)/stm32f4xx_hal_conf.mk
BSP_DIR = $(BSPs_DIR)/DISCOVERY-F407VG-BSP
BSP_LD_SCRIPT = $(BSP_DIR)/stm32f407vg_$(shell echo $(LOAD_MEMORY) | tr '[:upper:]' '[:lower:]').ldf

#################################
############## ELSE #############
#################################
else
$(error This board is not available for the flight software)
endif