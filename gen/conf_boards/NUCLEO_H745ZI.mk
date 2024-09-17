#################################
######### NUCLEO_H745ZI #########
#################################

# Chip Information
CHIP_VENDOR = ST
CHIP_FAMILLY = STM32H7xx
CHIP = STM32H745xx
MACH = cortex-m7
CORE_SELECT = -DCORE_CM7
FREERTOS_PORTABLE = ARM_CM4_MPU

# Debugger Information
OCD_DBG = interface/stlink.cfg
OCD_CHIP = target/stm32h7x.cfg

# HAL & BSP Information
HAL_SRCS_LIST = $(CONF_HALS_DIR)/stm32h7xx_hal_conf.mk
ifeq ($(CONFIG_LOAD_MEMORY_RAM), y)
BSP_LD_SCRIPT = $(BSP_DIR)/stm32h745zi_ram.ldf
ifeq ($(CONFIG_LOAD_MEMORY_FLASH), y)
BSP_LD_SCRIPT = $(BSP_DIR)/stm32h745zi_flash.ldf
else
$(error Board support RAM and FLASH load memory only)
endif