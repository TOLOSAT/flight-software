# Makefile with verification rules

##############################################
################ CHECKER FILES ###############
##############################################

CHECKER_SRCS =	$(MAIN_SRCS) \
				$(APPLICATION_SRCS) \
				$(LIBPUS_SRCS) \
				$(HAL_TOLOSAT_SRCS)
# We remove syscalls.c because it is used as an interface to the standard libraries and it is not MISRA Compliant
CHECKER_SRCS := $(filter-out %syscalls.c, $(CHECKER_SRCS)) 

CHECKER_INCS =	-I$(MAIN_INCDIR) \
				-I$(APPLICATION_INCDIR) \
				-I$(LIBPUS_INCDIR) \
				-I$(HAL_TOLOSAT_INCDIR)

##############################################
############### CHECKER CONFIGS ##############
##############################################

CHECKER_CMDS  = --enable=all # Enables all warnings
CHECKER_CMDS += --suppress=missingInclude # Disables missing include warnings
CHECKER_CMDS += --suppress=variableScope # Disables variable scope warnings
CHECKER_CMDS += --suppress=unusedFunction # Disables unused function warnings
CHECKER_CMDS += --inline-suppr # Allows to do suppress inside the code (inline)
CHECKER_CMDS += --addon=$(CONF_MISRA_DIR)/misra.json # Check MISRA C compliancee if misra settings are added 

##############################################
############## CHECKER COMMANDS ##############
##############################################

.PHONY += verif

verif :
	$(CHECKER) $(CHECKER_CMDS) -D$(CHIP) $(CHECKER_INCS) $(CHECKER_SRCS)