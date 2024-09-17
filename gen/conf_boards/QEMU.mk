#################################
############## QEMU #############
#################################

# Chip Information
FREERTOS_PORTABLE = ARM_CM4F

# Debugger Information
QEMU_MACHINE = mps2-an500

# HAL & BSP Information
HAL_SRCS_LIST = $(CONF_HALS_DIR)/cmsdk_hal_conf.mk
