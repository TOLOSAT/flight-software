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

# Valid Settings
VALID_LOAD_MEMORY = FLASH
VALID_CONSOLE_MODES = NONE UART FILE CIRCULAR-BUFFER
VALID_FS_MODES = NONE SPISD

# Debugger Information
OCD_DBG = interface/stlink.cfg
OCD_CHIP = target/stm32f4x.cfg

# HAL & BSP Information
HAL_SRCS_LIST = $(CONF_HALS_DIR)/stm32f4xx_hal_conf.mk
BSP_LD_SCRIPT = $(BSP_DIR)/stm32f407vg_$(shell echo $(LOAD_MEMORY) | tr '[:upper:]' '[:lower:]').ldf
