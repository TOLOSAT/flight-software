# Makefile with debugging rules

ifndef BUILD_DEBUG_MK
BUILD_DEBUG_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/paths.mk

##############################################
################ DEBUG CONFIGS ###############
##############################################

ifeq ($(CHIP_FAMILLY), STM32H7)
OCD_DBG = stlink
OCD_CHIP = stm32h7x
else ifeq ($(CHIP_FAMILLY), STM32F4)
OCD_DBG = stlink
OCD_CHIP = stm32f4x
else ifeq ($(CHIP_FAMILLY), CMSDK)
QEMU_MACHINE = mps2-an500
else
$(error This boards is not supported for debugging)
endif

# Upload command
ifeq ($(CONFIG_LOAD_MEMORY_FLASH), y)
OCD_LOAD = program
OCD_HW_BRKP_CMD =
else ifeq ($(CONFIG_LOAD_MEMORY_RAM), y)
OCD_LOAD = load_image
OCD_HW_BRKP_CMD = -c "gdb_breakpoint_override hard"
else
$(error Load memory can only be FLASH or RAM)
endif

##############################################
############ DEBUG/UPLOAD COMMANDS ###########
##############################################

.PHONY : debug gdb upload flash-erase set-boot-ram set-boot-flash

ifeq ($(BOARD), QEMU)
####################
####### QEMU #######
####################

# Debug Command
debug :
	@$(EMU) -machine $(QEMU_MACHINE) -cpu $(MACH) -m 16M -kernel $(TARGET) -nographic -serial mon:stdio -serial tcp:localhost:4444,server,nowait -s -S

# Start GDB
gdb:
	@$(GDB) --eval-command="target remote:1234" $(TARGET)

# Upload Command
upload :
	@$(EMU) -machine $(QEMU_MACHINE) -cpu $(MACH) -m 16M -kernel $(TARGET) -nographic -serial mon:stdio -serial tcp:localhost:4444,server,nowait

else
####################
### REAL TARGETS ###
####################

# Debug Command
debug :
	@$(OCD) -f interface/$(OCD_DBG).cfg -f target/$(OCD_CHIP).cfg -c init -c "reset init" $(OCD_HW_BRKP_CMD) -c "$(OCD_LOAD) $(TARGET)" -c "reset halt"

# Start GDB
gdb:
	@$(GDB) -ex "set pagination off" -ex "target extended-remote localhost:3333" $(TARGET)

# Upload Command
upload :
	@$(OCD) -f interface/$(OCD_DBG).cfg -f target/$(OCD_CHIP).cfg -c init  -c "reset init" -c "$(OCD_LOAD) $(TARGET)" -c "reset" -c "shutdown"

# Flash Erase Command
flash-erase:
	@$(OCD) -f interface/$(OCD_DBG).cfg -f target/$(OCD_CHIP).cfg -c init -c 'reset halt' -c '$(OCD_CHIP) mass_erase 0' -c "reset" -c "shutdown"

# Boot Option Commands
ifeq ($(CHIP_FAMILLY), STM32H7)
set-boot-ram:
	@$(OCD) -f interface/$(OCD_DBG).cfg -f target/$(OCD_CHIP).cfg -c init -c "reset init" -c "$(OCD_CHIP) option_write 0 0x44 0x00000000" -c "reset" -c "shutdown"

set-boot-flash:
	@$(OCD) -f interface/$(OCD_DBG).cfg -f target/$(OCD_CHIP).cfg -c init -c "reset init" -c "$(OCD_CHIP) option_write 0 0x44 0x00000800" -c "reset" -c "shutdown"
endif

endif

endif # BUILD_DEBUG_MK #