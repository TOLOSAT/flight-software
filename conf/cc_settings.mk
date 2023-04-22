# Makefile including build parameters

##############################################
################## C FLAGS ###################
##############################################

GENERIC_CFLAGS  = -c -mcpu=$(MACH) -std=gnu11 # Compiles with the processor using the GNU11 standard
GENERIC_CFLAGS += -D$(CHIP) # We indicate which chip we use
GENERIC_CFLAGS += -Wall # Enable all compiler warnings
GENERIC_CFLAGS += -Wextra # Enable extra compiler warnings
GENERIC_CFLAGS += -pedantic # Compiler generates warnings if your code uses any language feature that conflicts with strict ISO C or ISO C++
GENERIC_CFLAGS += --specs=nano.specs # Uses libraries related to newlib-nano which specialises in embedded systems
GENERIC_CFLAGS += $(FPU_SETTINGS) # Define if it uses FPU or not 
GENERIC_CFLAGS += -mthumb # Generate 16-bit instructions to optimise the process

##############################################
############### RELEASE FLAGS ################
##############################################

RELEASE_FLAGS  = -g0 # No debugging informations in the executable
RELEASE_FLAGS += -O2 # Sets the optimisation to level 2 (optimize code space and execution time)

##############################################
################# DBG FLAGS ##################
##############################################

DEBUG_FLAGS  = -g3 # Maximum debugging informations in the executable
DEBUG_FLAGS += -DDEBUG # Define DEBUG
DEBUG_FLAGS += -O0 # Sets the optimisation to level 0 (no optimisation)

##############################################
################## LD FLAGS ##################
##############################################

GENERIC_LDFLAGS = -mcpu=$(MACH) # Indicates the architecture of the target processor 
GENERIC_LDFLAGS += -T $(LINKER_SCRIPT) # Indicates the linker script file to use
GENERIC_LDFLAGS += --specs=nosys.specs # Disables semi-hosting (uses 'fake' I/O file and std I/O handlers)
GENERIC_LDFLAGS += -Wl,-Map=$(TARGET:.elf=.map) # Add a map file with the elf
GENERIC_LDFLAGS += -static # Does not link to dynamic libraries
GENERIC_LDFLAGS += -Wall # Enable all compiler warnings
GENERIC_LDFLAGS += -Wextra # Enable extra compiler warnings
GENERIC_LDFLAGS += -pedantic # Compiler generates warnings if your code uses any language feature that conflicts with strict ISO C or ISO C++
GENERIC_LDFLAGS += --specs=nano.specs # Uses libraries related to newlib-nano which is specialised in embedded systems
GENERIC_LDFLAGS += $(FPU_SETTINGS) # Define if it uses FPU or not 
GENERIC_LDFLAGS += -mthumb # Generate 16-bit instructions to optimise the process
GENERIC_LDFLAGS += -lc -lm # Includes lib c and lib math