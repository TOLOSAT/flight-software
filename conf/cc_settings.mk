# Makefile incluant les parametres de compilations

##############################################
################## C FLAGS ###################
##############################################

GENERIC_CFLAGS = -c -mcpu=$(MACH) -std=gnu11 #Compile avec le processeur en utilisant utilisant le standard C11
GENERIC_CFLAGS += --specs=nano.specs #Utilise les librairies liées à newlib-nano qui est spécialisée dans les systèmes embarqués.
GENERIC_CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard #Utilise les co-processeur qui gèrent les flottants
GENERIC_CFLAGS += -mthumb #Genere des instructions 16 pour optimiser le process
GENERIC_CFLAGS += -O0 #Regle l'optimisation au niveau 0 (par defaut)

##############################################
################# DBG FLAGS ##################
##############################################

GENERIC_DBGCFLAGS = -g3 -DDEBUG

##############################################
################## LD FLAGS ##################
##############################################

GENERIC_LDFLAGS = -mcpu=$(MACH) -T $(LINKER_SCRIPT) #Indique le processeur et utilise le fichier de linkage indiquée
GENERIC_LDFLAGS += --specs=nosys.specs #Desactive le semihosting (utilise des ‘faux’ fichier I/O and std I/O handlers)
GENERIC_LDFLAGS += -Wl,-Map=$(TARGET_MAP) #Ajoute la map du elf
GENERIC_LDFLAGS += -static #Ne fait pas de lien avec les librairies dynamiques
GENERIC_LDFLAGS += --specs=nano.specs #Utilise les librairies liées à newlib-nano qui est spécialisée dans les systèmes embarqués.
GENERIC_LDFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard #Utilise les co-processeur qui gèrent les flottants
GENERIC_LDFLAGS += -mthumb #Genere des instructions 16 pour optimiser le process
GENERIC_LDFLAGS += -lc -lm #Inclu la lib c, la lib math et la lib gcc