# Makefile including build parameters

ifndef CC_SETTINGS_MK
CC_SETTINGS_MK := yes

##############################################
################## C FLAGS ###################
##############################################

PROJECT_CFLAGS  = -c -mcpu=$(MACH) -std=gnu11 # Compiles with the processor using the GNU11 standard
PROJECT_CFLAGS += -ffunction-sections -fdata-sections # Place each symbol in its own section, it will be used to optimise the code.
PROJECT_CFLAGS += -D$(CHIP) -D$(CHIP_FAMILLY) $(CORE_SELECT) # Defines which chip, chip familly and core used
PROJECT_CFLAGS += -Wall # Enable all compiler warnings
PROJECT_CFLAGS += -Wextra # Enable extra compiler warnings
PROJECT_CFLAGS += -Werror # All warnings are seen as compilation errors
PROJECT_CFLAGS += $(FPU_SETTINGS) # Define if FPU is soft or hard and which fpu is used (if any)
PROJECT_CFLAGS += -mthumb # Generate 16-bit instructions to optimise the process
PROJECT_CFLAGS += -MMD -MP # Generate dependancy files
PROJECT_CFLAGS += --specs=nosys.specs # Indicates absence of system, as a result system calls are disabled
PROJECT_CFLAGS += --specs=nano.specs # Uses libraries related to newlib-nano which is specialised for embedded systems

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

PROJECT_LDFLAGS  = -mcpu=$(MACH) # Indicates the architecture of the target processor 
PROJECT_LDFLAGS += -Wl,-Map=$(TARGET:.elf=.map) # Add a map file with the elf
PROJECT_LDFLAGS += -Wl,--print-memory-usage # Print the memory usage according to the linkerscript
PROJECT_LDFLAGS += -Wl,--gc-sections # Eliminates unused sections
PROJECT_LDFLAGS += -static # Do not link dynamically libraries
PROJECT_LDFLAGS += -Wall # Enable all compiler warnings
PROJECT_LDFLAGS += -Wextra # Enable extra compiler warnings
PROJECT_LDFLAGS += -Werror # All warnings are seen as compilation errors
PROJECT_LDFLAGS += $(FPU_SETTINGS) # Which fpu is used (if any)
PROJECT_LDFLAGS += -mthumb # Generate 16-bit instructions to optimise the process
PROJECT_LDFLAGS += --specs=nosys.specs # Indicates absence of system, as a result system calls are disabled
PROJECT_LDFLAGS += --specs=nano.specs # Uses libraries related to newlib-nano which is specialised for embedded systems

endif # CC_SETTINGS_MK #