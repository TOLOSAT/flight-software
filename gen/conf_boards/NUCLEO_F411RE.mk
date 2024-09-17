#################################
######### NUCLEO_F411RE #########
#################################

# Chip Information
FREERTOS_PORTABLE = ARM_CM4F

# Debugger Information
OCD_DBG = interface/stlink.cfg
OCD_CHIP = target/stm32f4x.cfg

# HAL & BSP Information
HAL_SRCS_LIST = $(CONF_HALS_DIR)/stm32f4xx_hal_conf.mk
