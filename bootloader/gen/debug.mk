# Makefile with debugging rules

ifndef BUILD_DEBUG_MK
BUILD_DEBUG_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk

##############################################
################ DEBUG CONFIGS ###############
##############################################

ifeq ($(CHIP_FAMILLY), STM32H7)
OCD_DBG = interface/stlink.cfg
OCD_CHIP = target/stm32h7x.cfg
else
$(error This boards is not supported for debugging)
endif

# Upload Commands
UPLOAD_CMDS  = -c "reset init"
UPLOAD_CMDS += -c "program $(TARGET)"
UPLOAD_CMDS += -c "reset"
UPLOAD_CMDS += -c "shutdown"

# Debug Commands
DBG_CMDS  = -c "reset init"
DBG_CMDS += -c "program $(TARGET)"
DBG_CMDS += -c "reset halt"

##############################################
############### DEBUG COMMANDS ###############
##############################################

.PHONY += debug gdb upload

debug :
	@$(OCD) -f $(OCD_DBG) -f $(OCD_CHIP) -c init $(DBG_CMDS)

gdb:
	@$(GDB) -ex "set pagination off" -ex "target extended-remote localhost:3333" $(TARGET)

upload :
	@$(OCD) -f $(OCD_DBG) -f $(OCD_CHIP) -c init $(UPLOAD_CMDS)

endif # BUILD_DEBUG_MK #