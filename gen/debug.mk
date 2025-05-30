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
OCD_DBG = stlink
OCD_CHIP = stm32h7x
else
$(error This boards is not supported for debugging)
endif

##############################################
############### DEBUG COMMANDS ###############
##############################################

.PHONY : debug gdb upload

# Debug Command
debug :
	@echo $(OCD) -f interface/$(OCD_DBG).cfg -f target/$(OCD_CHIP).cfg -c init -c "reset init" -c "program $(TARGET)" -c "reset halt"

# Start GDB
gdb:
	@$(GDB) -ex "set pagination off" -ex "target extended-remote localhost:3333" $(TARGET)

# Upload Command
upload :
	@$(OCD) -f interface/$(OCD_DBG).cfg -f target/$(OCD_CHIP).cfg -c init  -c "reset init" -c "program $(TARGET)" -c "reset" -c "shutdown"

endif # BUILD_DEBUG_MK #