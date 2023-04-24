#Makefile parametrant les variables de board

ifeq ($(BOARD), NUCLEO-F411RE)
	CHIP_VENDOR = ST
	CHIP_FAMILLY = STM32F4xx
	CHIP = STM32F411xE
	MACH = cortex-m4
	FPU_SETTINGS = -mfpu=fpv4-sp-d16 -mfloat-abi=hard
	OCD_DBG = interface/stlink-v2-1.cfg
	OCD_CHIP = target/stm32f4x.cfg
else ifeq ($(BOARD), NUCLEO-F103RB)
	CHIP_VENDOR = ST
	CHIP_FAMILLY = STM32F1xx
	CHIP = STM32F103xB
	MACH = cortex-m3
	FPU_SETTINGS = -mfloat-abi=soft
	OCD_DBG = interface/stlink-v2-1.cfg
	OCD_CHIP = target/stm32f1x.cfg
else
	print := $(error This board is not available for the flight software)
endif