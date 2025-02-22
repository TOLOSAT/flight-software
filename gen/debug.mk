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
else ifeq ($(CHIP_FAMILLY), STM32F4)
OCD_DBG = interface/stlink.cfg
OCD_CHIP = target/stm32f4x.cfg
else ifeq ($(CHIP_FAMILLY), CMSDK)
QEMU_MACHINE = mps2-an500
else
$(error This boards is not supported for debugging)
endif

# Upload Commands
ifeq ($(CONFIG_LOAD_MEMORY_FLASH), y)
UPLOAD_CMDS  = -c "reset init"
UPLOAD_CMDS += -c "program $(TARGET)"
UPLOAD_CMDS += -c "reset"
UPLOAD_CMDS += -c "shutdown"
else ifeq ($(CONFIG_LOAD_MEMORY_RAM), y)
UPLOAD_CMDS  = -c "reset init"
UPLOAD_CMDS += -c "load_image $(TARGET)"
UPLOAD_CMDS += -c "reset"
UPLOAD_CMDS += -c "shutdown"
else
$(error Load memory can only be FLASH or RAM)
endif

# Debug Commands
ifeq ($(CONFIG_LOAD_MEMORY_FLASH), y)
DBG_CMDS  = -c "reset init"
DBG_CMDS += -c "program $(TARGET)"
DBG_CMDS += -c "reset halt"
else ifeq ($(CONFIG_LOAD_MEMORY_RAM), y)
DBG_CMDS  = -c "reset init"
DBG_CMDS += -c "gdb_breakpoint_override hard"
DBG_CMDS += -c "load_image $(TARGET)"
DBG_CMDS += -c "reset halt"
else
$(error Load memory can only be FLASH or RAM)
endif

##############################################
############### DEBUG COMMANDS ###############
##############################################

.PHONY += debug gdb upload

ifeq ($(BOARD), QEMU)
debug :
	@$(EMU) -machine $(QEMU_MACHINE) -cpu $(MACH) -m 16M -kernel $(TARGET) -nographic -serial mon:stdio -serial tcp:localhost:4444,server,nowait -s -S

gdb:
	@$(GDB) --eval-command="target remote:1234" $(TARGET)

upload :
	@$(EMU) -machine $(QEMU_MACHINE) -cpu $(MACH) -m 16M -kernel $(TARGET) -nographic -serial mon:stdio -serial tcp:localhost:4444,server,nowait
else
debug :
	@$(OCD) -f $(OCD_DBG) -f $(OCD_CHIP) -c init $(DBG_CMDS)

gdb:
	@$(GDB) -ex "set pagination off" -ex "target extended-remote localhost:3333" $(TARGET)

upload :
	@$(OCD) -f $(OCD_DBG) -f $(OCD_CHIP) -c init $(UPLOAD_CMDS)
endif

endif # BUILD_DEBUG_MK #