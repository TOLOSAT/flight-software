# Makefile with debugging rules

include gen/settings.mk
include gen/path.mk

##############################################
################ OCD CONFIGS #################
##############################################

# OCD Commands Variables
CHIP_FAMILLY_LOWER = $(shell echo $(CHIP_FAMILLY) | tr '[:upper:]' '[:lower:]' | sed 's/.$$//')

# Upload Commands
ifeq ($(LOAD_MEMORY), FLASH)
UPLOAD_CMDS  = -c 'reset init'
UPLOAD_CMDS += -c 'program $(TARGET)'
UPLOAD_CMDS += -c 'reset'
UPLOAD_CMDS += -c 'shutdown'
else ifeq ($(LOAD_MEMORY), RAM)
UPLOAD_CMDS  = -c 'reset init'
UPLOAD_CMDS += -c 'load_image $(TARGET)'
UPLOAD_CMDS += -c 'reset'
UPLOAD_CMDS += -c 'shutdown'
else
$(error Load memory can only be FLASH or RAM)
endif

# Debug Commands
ifeq ($(LOAD_MEMORY), FLASH)
DBG_CMDS  = -c 'reset init'
DBG_CMDS += -c 'program $(TARGET)'
DBG_CMDS += -c 'reset halt'
else ifeq ($(LOAD_MEMORY), RAM)
DBG_CMDS  = -c 'reset init'
DBG_CMDS += -c 'gdb_breakpoint_override hard'
DBG_CMDS += -c 'load_image $(TARGET)'
DBG_CMDS += -c 'reset halt'
else
$(error Load memory can only be FLASH or RAM)
endif

# Erase Commands
ERASE_CMDS  = -c 'reset halt'
ERASE_CMDS += -c '$(CHIP_FAMILLY_LOWER) mass_erase 0'
ERASE_CMDS += -c 'reset'
ERASE_CMDS += -c 'shutdown'

##############################################
############### DEBUG COMMANDS ###############
##############################################

.PHONY += debug gdb upload flash-erase

ifeq ($(BOARD), QEMU)
debug :
	$(EMU) -machine $(QEMU_MACHINE) -cpu $(MACH) -m 16M -kernel $(TARGET) -nographic -serial mon:stdio -s -S

gdb:
	$(GDB) --eval-command="target remote:1234" $(TARGET)

upload :
	$(EMU) -machine $(QEMU_MACHINE) -cpu $(MACH) -m 16M -kernel $(TARGET) -nographic -serial mon:stdio
else
debug :
	$(OCD) -f $(OCD_DBG) -f $(OCD_CHIP) -c init $(DBG_CMDS)

gdb:
	$(GDB) -ex 'set pagination off' -ex 'target extended-remote localhost:3333' $(TARGET)

upload :
	$(OCD) -f $(OCD_DBG) -f $(OCD_CHIP) -c init $(UPLOAD_CMDS)

flash-erase :
	$(OCD) -f $(OCD_DBG) -f $(OCD_CHIP) -c init $(ERASE_CMDS)
endif