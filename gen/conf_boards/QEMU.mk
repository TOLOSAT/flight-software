#################################
############## QEMU #############
#################################

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
ifeq ($(CONFIG_LOAD_MEMORY_RAM), y)
BSP_LD_SCRIPT = $(BSP_DIR)/mps2_an500_ram.ldf
else
$(error Board support RAM load memory only)
endif
