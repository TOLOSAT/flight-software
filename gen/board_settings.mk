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
# Debugger Information
OCD_DBG = interface/stlink.cfg
OCD_CHIP = target/stm32h7x.cfg
# HAL & BSP Information
HAL_SRCS_LIST = $(CONF_HALS_DIR)/stm32h7xx_hal_conf.mk
BSP_DIR = $(BSPs_DIR)/ART-PI-BSP
LINKER_SCRIPT = $(BSP_DIR)/stm32h750xb_boot.ld

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
# Debugger Information
OCD_DBG = interface/stlink.cfg
OCD_CHIP = target/stm32h7x.cfg
# HAL & BSP Information
HAL_SRCS_LIST = $(CONF_HALS_DIR)/stm32h7xx_hal_conf.mk
BSP_DIR = $(BSPs_DIR)/NUCLEO-H745ZI-BSP
LINKER_SCRIPT = $(BSP_DIR)/stm32h745zi_boot.ld

#################################
############## ELSE #############
#################################
else
$(error This board is not available for the flight software)
endif