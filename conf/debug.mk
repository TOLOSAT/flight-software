# Makefile avec les regles pour debugger

.PHONY += debug gdb flash

debug : $(TARGET)
	$(OCD) -f $(OCD_DBG) -f $(OCD_CHIP) -c init $(DBG_CMDS)

gdb:
	$(GDB) --eval-command="target remote localhost:3333" $(TARGET)

flash : $(TARGET)
	$(OCD) -f $(OCD_DBG) -f $(OCD_CHIP) -c init $(FLASH_CMDS)