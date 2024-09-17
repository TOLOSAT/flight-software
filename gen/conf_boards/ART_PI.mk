#################################
############# ART_PI ############
#################################

# Chip Information
FREERTOS_PORTABLE = ARM_CM4_MPU

# Debugger Information
OCD_DBG = interface/stlink.cfg
OCD_CHIP = target/stm32h7x.cfg

# HAL & BSP Information
HAL_SRCS_LIST = $(CONF_HALS_DIR)/stm32h7xx_hal_conf.mk
