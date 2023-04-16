# Makefile incluant les parametres de compilations

##############################################
################## C FLAGS ###################
##############################################

CFLAGS = -c -mcpu=$(MACH) -std=gnu11 #Compile avec le processeur en utilisant utilisant le standard C11
CFLAGS += --specs=nano.specs #Utilise les librairies liées à newlib-nano qui est spécialisée dans les systèmes embarqués.
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard #Utilise les co-processeur qui gèrent les flottants
CFLAGS += -mthumb #Genere des instructions 16 pour optimiser le process
CFLAGS += -O0 #Regle l'optimisation au niveau 0 (par defaut)

##############################################
################# DBG FLAGS ##################
##############################################

DBGCFLAGS = -g3 -DDEBUG

##############################################
################# INC FLAGS ##################
##############################################

INCFLAGS = -I$(MAIN_INCDIR)
INCFLAGS += -I$(CMSIS_INCDIR)
INCFLAGS += -I$(CMSIS_INCDIR_DEVICE) -D $(CHIP)
INCFLAGS += -I$(HAL_INCDIR)
INCFLAGS += -I$(HAL_INCDIR)/Legacy
INCFLAGS += -I$(HAL_TOLOSAT_INCDIR)
INCFLAGS += -I$(BSP_INCDIR)

##############################################
################## LD FLAGS ##################
##############################################

LDFLAGS = -mcpu=$(MACH) -T $(LINKER_SCRIPT) #Indique le processeur et utilise le fichier de linkage indiquée
LDFLAGS += --specs=nosys.specs #Desactive le semihosting (utilise des ‘faux’ fichier I/O and std I/O handlers)
LDFLAGS += -Wl,-Map=$(TARGET_MAP) #Ajoute la map du elf
LDFLAGS += -static #Ne fait pas de lien avec les librairies dynamiques
LDFLAGS += --specs=nano.specs #Utilise les librairies liées à newlib-nano qui est spécialisée dans les systèmes embarqués.
LDFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard #Utilise les co-processeur qui gèrent les flottants
LDFLAGS += -mthumb #Genere des instructions 16 pour optimiser le process
LDFLAGS += -lc -lm #Inclu la lib c, la lib math et la lib gcc