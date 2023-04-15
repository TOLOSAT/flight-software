# Makefile de génération du flight software

##############################################
################## PROJECT ###################
##############################################

PROJ_NAME=blink
CHIP_VENDOR = ST
CHIP_FAMILLY = STM32F4xx
CHIP = STM32F407xx
MACH = cortex-m4
HOST_OS = $(shell uname)

##############################################
################### TOOLS ####################
##############################################

# Tools
ifeq ($(HOST_OS), Darwin) 
	CC = /usr/local/bin/arm-none-eabi-gcc
	GDB = /usr/local/bin/gdb
	OCD = /usr/local/bin/openocd
endif
ifeq ($(HOST_OS), Linux)
	CC = /usr/bin/arm-none-eabi-gcc
	GDB = /usr/bin/gdb-multiarch
	OCD = /usr/bin/openocd
endif

##############################################
################ DIRECTORIES #################
##############################################

# Directories
WORKSPACE = .
BUILD_DIR = $(WORKSPACE)/build
CORE_DIR = $(WORKSPACE)/core
MAIN_DIR = $(CORE_DIR)/main
TOOLS_DIR = $(WORKSPACE)/tools
CMSIS_DIR = $(TOOLS_DIR)/CMSIS
HALs_DIR = $(TOOLS_DIR)/HALs
HAL_DIR = $(HALs_DIR)/HAL-STM32F4
TARGET_DIR = $(BUILD_DIR)/target
CONF_DIR = $(WORKSPACE)/conf
LINKER_SCRIPTS_DIR = $(CONF_DIR)/linkerscripts

##############################################
################### MAIN #####################
##############################################

# Main Directories
MAIN_INCDIR = $(MAIN_DIR)/inc
MAINCONF_INCDIR = $(MAIN_DIR)/inc/conf
MAIN_SRCDIR = $(MAIN_DIR)/src
MAINCONF_SRCDIR = $(MAIN_DIR)/src/conf
MAIN_OBJDIR = $(BUILD_DIR)/main

# Files
MAIN_SRCS = $(wildcard $(MAIN_SRCDIR)/*.c $(MAINCONF_SRCDIR)/*.c)
MAIN_OBJS = $(MAIN_SRCS:.c=.o)
MAIN_OBJS := $(subst $(MAIN_SRCDIR)/,$(MAIN_OBJDIR)/,$(MAIN_OBJS))
TARGET = $(TARGET_DIR)/final.elf
TARGET_MAP = $(TARGET:.elf=.map)

LINKER_SCRIPT = $(LINKER_SCRIPTS_DIR)/stm32f4_ls.ld

##############################################
################### CMSIS #####################
##############################################

# CMSIS Directories
CMSIS_INCDIR = $(CMSIS_DIR)/Include
CMSIS_INCDIR_DEVICE = $(CMSIS_DIR)/Device/$(CHIP_VENDOR)/$(CHIP_FAMILLY)/Include
CMSIS_SRCDIR_DEVICE = $(CMSIS_DIR)/Device/$(CHIP_VENDOR)/$(CHIP_FAMILLY)/Source
CMSIS_OBJDIR = $(BUILD_DIR)/cmsis

# CMSIS Files
CMSIS_SRCS = $(wildcard $(CMSIS_SRCDIR_DEVICE)/*.c)
CMSIS_OBJS = $(CMSIS_SRCS:.c=.o)
CMSIS_OBJS := $(subst $(CMSIS_SRCDIR_DEVICE)/,$(CMSIS_OBJDIR)/,$(CMSIS_OBJS))

##############################################
#################### HAL #####################
##############################################

# HAL Directories
HAL_INCDIR = $(HAL_DIR)/Inc
HAL_SRCDIR = $(HAL_DIR)/Src
HAL_OBJDIR = $(BUILD_DIR)/hal

# HAL Files
HAL_SRCS = $(wildcard $(HAL_SRCDIR)/*.c $(HAL_SRCDIR)/Legacy/*.c)
HAL_OBJS = $(HAL_SRCS:.c=.o)
HAL_OBJS := $(subst $(HAL_SRCDIR)/,$(HAL_OBJDIR)/,$(HAL_OBJS))

##############################################
################### FLAGS ####################
##############################################

# Compiler Flags
CFLAGS = -c -mcpu=$(MACH) -std=gnu11 #Compile avec le processeur en utilisant utilisant le standard C11
CFLAGS += --specs=nano.specs #Utilise les librairies liées à newlib-nano qui est spécialisée dans les systèmes embarqués.
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard #Utilise les co-processeur qui gèrent les flottants
CFLAGS += -mthumb #Genere des instructions 16 pour optimiser le process
CFLAGS += -O0 #Regle l'optimisation au niveau 0 (par defaut)

# Debug Flags
DBGCFLAGS = -g3 -DDEBUG

#Includes
INCFLAGS = -I$(MAIN_INCDIR) -I$(MAINCONF_INCDIR)
INCFLAGS += -I$(CMSIS_INCDIR)
INCFLAGS += -I$(CMSIS_INCDIR_DEVICE) -D $(CHIP)
INCFLAGS += -I$(HAL_INCDIR)
INCFLAGS += -I$(HAL_INCDIR)/Legacy

# Linker Flags
LDFLAGS = -mcpu=$(MACH) -T $(LINKER_SCRIPT) #Indique le processeur et utilise le fichier de linkage indiquée
LDFLAGS += --specs=nosys.specs #Desactive le semihosting (utilise des ‘faux’ fichier I/O and std I/O handlers)
LDFLAGS += -Wl,-Map=$(TARGET_MAP) #Ajoute la map du elf
LDFLAGS += -static #Ne fait pas de lien avec les librairies dynamiques
LDFLAGS += --specs=nano.specs #Utilise les librairies liées à newlib-nano qui est spécialisée dans les systèmes embarqués.
LDFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard #Utilise les co-processeur qui gèrent les flottants
LDFLAGS += -mthumb #Genere des instructions 16 pour optimiser le process
LDFLAGS += -lc -lm #Inclu la lib c, la lib math et la lib gcc

##############################################
################ OCD CONFIGS #################
##############################################

# OCD configuration
OCD_DBG = interface/stlink-v2-1.cfg
OCD_CHIP = target/stm32f4x.cfg

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
################### MAKE #####################
##############################################

.PHONY = all clean clean-all debug flash gdb echoes hal clean-hal

all : $(TARGET)

$(CMSIS_OBJDIR)/%.o : $(CMSIS_SRCDIR_DEVICE)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCFLAGS) $(DBGCFLAGS) $^ -o $@ 

$(HAL_OBJDIR)/%.o : $(HAL_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCFLAGS) $(DBGCFLAGS) $^ -o $@ 

$(MAIN_OBJDIR)/%.o : $(MAIN_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCFLAGS) $(DBGCFLAGS) $^ -o $@

$(TARGET) : $(CMSIS_OBJS) $(HAL_OBJS) ${MAIN_OBJS} 
	mkdir -p $(@D)
	$(CC) $^ $(LDFLAGS) -o $@
	@echo "*****************************"
	@echo "***   Target Build Done   ***"
	@echo "*****************************"

clean : clean-main

clean-main :
	rm -rf $(TARGET) $(MAIN_OBJDIR)

clean-all : 
	rm -rf $(BUILD_DIR)

debug : $(TARGET)
	$(OCD) -f $(OCD_DBG) -f $(OCD_CHIP) -c init $(DBG_CMDS)

flash : $(TARGET)
	$(OCD) -f $(OCD_DBG) -f $(OCD_CHIP) -c init $(FLASH_CMDS)

gdb:
	$(GDB) --eval-command="target remote localhost:3333" $(TARGET)

echoes :
	@echo "MAIN SRCS : \n$(MAIN_SRCS)"
	@echo "MAIN OBJS : \n$(MAIN_OBJS)"