# Makefile incluant les parametres de debuggage

##############################################
################ OCD CONFIGS #################
##############################################

# OCD configuration
OCD_DBG = interface/stlink-v2-1.cfg
OCD_CHIP = target/stm32f4x.cfg

# Flash Commands
FLASH_CMDS += -c 'reset halt'
FLASH_CMDS += -c 'program $(TARGET)'
FLASH_CMDS += -c 'reset'
FLASH_CMDS += -c 'shutdown'

# Debug Commands
DBG_CMDS += -c 'reset halt'
DBG_CMDS += -c 'program $(TARGET)'
DBG_CMDS += -c 'reset halt'