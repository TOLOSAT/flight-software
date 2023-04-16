#Makefile parametrant les variables de board

ifeq ($(BOARD), NUCLEO-F411RE)
	CHIP_VENDOR = ST
	CHIP_FAMILLY = STM32F4xx
	CHIP = STM32F411xE
	MACH = cortex-m4
else
	print := $(error This board is not available for the flight software)
endif