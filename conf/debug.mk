# Makefile avec les regles pour debugger

##############################################
################ OCD CONFIGS #################
##############################################

# Flash Commands
FLASH_CMDS += -c 'reset halt'
FLASH_CMDS += -c 'program $(TARGET)'
FLASH_CMDS += -c 'reset'
FLASH_CMDS += -c 'shutdown'

# Debug Commands
DBG_CMDS += -c 'reset halt'
DBG_CMDS += -c 'program $(TARGET)'
DBG_CMDS += -c 'reset halt'

##############################################
################ OCD COMMANDS ################
##############################################

.PHONY += debug gdb flash

debug :
	$(OCD) -f $(OCD_DBG) -f $(OCD_CHIP) -c init $(DBG_CMDS)

gdb:
	$(GDB) --eval-command="target remote localhost:3333" $(TARGET)

flash :
	$(OCD) -f $(OCD_DBG) -f $(OCD_CHIP) -c init $(FLASH_CMDS)