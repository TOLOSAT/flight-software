#################################
######## DISCOVERY_F407VG #######
#################################

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
ifeq ($(CONFIG_LOAD_MEMORY_FLASH), y)
BSP_LD_SCRIPT = $(BSP_DIR)/stm32f407vg_flash.ldf
else
$(error Board support FLASH load memory only)
endif