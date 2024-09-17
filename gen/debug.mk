# Makefile with debugging rules

##############################################
################ OCD CONFIGS #################
##############################################

# Upload Commands
UPLOAD_CMDS  = -c 'reset init'
UPLOAD_CMDS += -c 'program $(TARGET)'
UPLOAD_CMDS += -c 'reset'
UPLOAD_CMDS += -c 'shutdown'

# Debug Commands
DBG_CMDS  = -c 'reset init'
DBG_CMDS += -c 'program $(TARGET)'
DBG_CMDS += -c 'reset halt'

##############################################
################ OCD COMMANDS ################
##############################################

.PHONY += debug gdb upload

debug :
	$(OCD) -f $(OCD_DBG) -f $(OCD_CHIP) -c init $(DBG_CMDS)

gdb:
	$(GDB) -ex 'set pagination off' -ex 'target extended-remote localhost:3333' $(TARGET)

upload :
	$(OCD) -f $(OCD_DBG) -f $(OCD_CHIP) -c init $(UPLOAD_CMDS)
