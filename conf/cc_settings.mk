# Makefile including build parameters

##############################################
################## C FLAGS ###################
##############################################

GENERIC_CFLAGS  = -c -mcpu=$(MACH) -std=gnu11 # Compiles with the processor using the GNU11 standard
GENERIC_CFLAGS += -D$(CHIP) # We indicate which chip we use
GENERIC_CFLAGS += -D$(BOARD) # We indicate which board we use
GENERIC_CFLAGS += $(CORE_SELECT) # Define which core to use (if there is more than one core)
GENERIC_CFLAGS += -Werror # All warnings are seen as compilation errors
GENERIC_CFLAGS += -Wall # Enable all compiler warnings
GENERIC_CFLAGS += -Wextra # Enable extra compiler warnings
GENERIC_CFLAGS += -pedantic # Compiler generates warnings if your code uses any language feature that conflicts with strict ISO C or ISO C++
GENERIC_CFLAGS += $(FPU_SETTINGS) # Define if we use FPU or not
GENERIC_CFLAGS += $(CACHE_SETTINGS) # Define if we use cache or not
GENERIC_CFLAGS += $(MPU_SETTINGS) # Define if we use MPU or not
GENERIC_CFLAGS += $(IT_VECTOR_SETTINGS) # Define where to put interrupt vector (SRAM or Flash)
GENERIC_CFLAGS += -mthumb # Generate 16-bit instructions to optimise the process
GENERIC_CFLAGS += --specs=nosys.specs # Indicates absence of system, as a result system calls are disabled
GENERIC_CFLAGS += --specs=nano.specs # Uses libraries related to newlib-nano which is specialised for embedded systems

##############################################
############### RELEASE FLAGS ################
##############################################

RELEASE_FLAGS  = -g0 # No debugging informations in the executable
RELEASE_FLAGS += -O3 # Sets the optimisation to level 3 (optimize code space and execution time)

##############################################
################# DBG FLAGS ##################
##############################################

DEBUG_FLAGS  = -g3 # Maximum debugging informations in the executable
DEBUG_FLAGS += -DDEBUG # Define DEBUG
DEBUG_FLAGS += -O0 # Sets the optimisation to level 0 (no optimisations)

##############################################
################## LD FLAGS ##################
##############################################

GENERIC_LDFLAGS  = -mcpu=$(MACH) # Indicates the architecture of the target processor 
GENERIC_LDFLAGS += -T $(LINKER_SCRIPT) # Indicates the linker script file to use
GENERIC_LDFLAGS += -Wl,-Map=$(TARGET:.elf=.map) # Add a map file with the elf
GENERIC_LDFLAGS += -Wl,--print-memory-usage # Print the memory usage according to the linkerscript
GENERIC_LDFLAGS += -static # Does not link to dynamic libraries
GENERIC_LDFLAGS += -Wall # Enable all compiler warnings
GENERIC_LDFLAGS += -Wextra # Enable extra compiler warnings
GENERIC_LDFLAGS += -pedantic # Compiler generates warnings if your code uses any language feature that conflicts with strict ISO C or ISO C++
GENERIC_LDFLAGS += $(FPU_SETTINGS) # Define if it uses FPU or not
GENERIC_LDFLAGS += $(CACHE_SETTINGS) # Define if it uses cache or not
GENERIC_LDFLAGS += $(MPU_SETTINGS) # Define if we use MPU or not
GENERIC_LDFLAGS += $(IT_VECTOR_SETTINGS) # Define where to put interrupt vector (SRAM or Flash)
GENERIC_LDFLAGS += -mthumb # Generate 16-bit instructions to optimise the process
GENERIC_LDFLAGS += --specs=nosys.specs # Indicates absence of system, as a result system calls are disabled
GENERIC_LDFLAGS += --specs=nano.specs # Uses libraries related to newlib-nano which is specialised for embedded systems