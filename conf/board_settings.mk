#Makefile parametrant les variables de board

ifeq ($(BOARD), NUCLEO-F411RE)
CHIP_VENDOR = ST
CHIP_FAMILLY = STM32F4xx
CHIP = STM32F411xE
MACH = cortex-m4
FPU_SETTINGS = -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CORE_SELECT =
OCD_DBG = interface/stlink-v2-1.cfg
OCD_CHIP = target/stm32f4x.cfg
HAL_SRCS_LIST = $(CONF_HALS_DIR)/stm32f4xx_hal_conf.mk
else ifeq ($(BOARD), NUCLEO-F103RB)
CHIP_VENDOR = ST
CHIP_FAMILLY = STM32F1xx
CHIP = STM32F103xB
MACH = cortex-m3
FPU_SETTINGS = -mfloat-abi=soft
CORE_SELECT =
OCD_DBG = interface/stlink-v2-1.cfg
OCD_CHIP = target/stm32f1x.cfg
HAL_SRCS_LIST = $(CONF_HALS_DIR)/stm32f1xx_hal_conf.mk
else ifeq ($(BOARD), NUCLEO-H745ZI)
CHIP_VENDOR = ST
CHIP_FAMILLY = STM32H7xx
CHIP = STM32H745xx
MACH = cortex-m7
FPU_SETTINGS = -mfpu=fpv5-d16 -mfloat-abi=hard
CORE_SELECT = -DCORE_CM7
OCD_DBG = interface/stlink-v2-1.cfg
OCD_CHIP = target/stm32h7x.cfg
HAL_SRCS_LIST = $(CONF_HALS_DIR)/stm32h7xx_hal_conf.mk
else
$(error This board is not available for the flight software)
endif