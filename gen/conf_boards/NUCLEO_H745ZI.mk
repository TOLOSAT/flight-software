#################################
######### NUCLEO_H745ZI #########
#################################

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

# Valid Settings
VALID_LOAD_MEMORY = FLASH RAM
VALID_CONSOLE_MODES = NONE UART FILE CIRCULAR-BUFFER
VALID_FS_MODES = NONE SPISD

# Debugger Information
OCD_DBG = interface/stlink.cfg
OCD_CHIP = target/stm32h7x.cfg

# HAL & BSP Information
HAL_SRCS_LIST = $(CONF_HALS_DIR)/stm32h7xx_hal_conf.mk
BSP_LD_SCRIPT = $(BSP_DIR)/stm32h745zi_$(shell echo $(LOAD_MEMORY) | tr '[:upper:]' '[:lower:]').ldf
