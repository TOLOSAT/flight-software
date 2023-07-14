# Makefile with verification rules

##############################################
################ CHECKER FILES ###############
##############################################

CHECKER_SRCS =	$(MAIN_SRCS) \
				$(APPLICATION_SRCS) \
				$(LIBPUS_SRCS) \
				$(HAL_TOLOSAT_SRCS) \
				$(BSP_SRCS)

CHECKER_INCS =	-I$(MAIN_INCDIR) \
				-I$(APPLICATION_INCDIR) \
				-I$(LIBPUS_INCDIR) \
				-I$(HAL_TOLOSAT_INCDIR) \
				-I$(BSP_INCDIR)

##############################################
############### CHECKER CONFIGS ##############
##############################################

CHECKER_CMDS  = --enable=all # Enables all warnings
CHECKER_CMDS += --suppress=missingInclude # Disables missing include warnings
CHECKER_CMDS += --suppress=variableScope # Disables variable scope warnings
CHECKER_CMDS += --suppress=*:$(BSP_SRCDIR)/system*.c # Disables all warning for system file (ST wrote this file)
CHECKER_CMDS += --suppress=*:$(BSP_SRCDIR)/*hal_msp.c # Disables all warning for system file (ST wrote this file)
CHECKER_CMDS += --suppress=unusedFunction # Disables unused function warnings
CHECKER_CMDS += --inline-suppr # Allows to do suppress inside the code (inline)
CHECKER_CMDS += --addon=conf/conf_misra/misra.json

##############################################
############## CHECKER COMMANDS ##############
##############################################

.PHONY += verif

verif :
	$(CHECKER) $(CHECKER_CMDS) -D$(CHIP) $(CHECKER_INCS) $(CHECKER_SRCS)