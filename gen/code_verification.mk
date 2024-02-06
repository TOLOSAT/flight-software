# Makefile with verification rules

##############################################
################ CHECKER FILES ###############
##############################################

CHECKER_SRCS =	$(CORE_SRCS) \
				$(APPLICATION_SRCS) \
				$(LIBPUS_SRCS) \
				$(TOLOSAT_FS_SRCS) \
				$(HAL_TOLOSAT_SRCS)
# We remove syscalls.c because it is used as an interface to the standard libraries and it is not MISRA Compliant
CHECKER_SRCS := $(filter-out %syscalls.c, $(CHECKER_SRCS)) 

CHECKER_INCS =	-I$(CORE_INCDIR) \
				-I$(APPLICATION_INCDIR) \
				-I$(LIBPUS_INCDIR) \
				-I$(TOLOSAT_FS_INCDIR) \
				-I$(HAL_TOLOSAT_INCDIR)

##############################################
############### CHECKER CONFIGS ##############
##############################################

CHECKER_CMDS  = --enable=all # Enables all warnings
CHECKER_CMDS += --suppress=missingInclude # Disables missing includes warnings
CHECKER_CMDS += --suppress=missingIncludeSystem # Disables missing standard libs includes warnings
CHECKER_CMDS += --suppress=unusedFunction # Disables unused function warnings
CHECKER_CMDS += --inline-suppr # Allows to do suppress inside the code (inline)
CHECKER_CMDS += --addon=$(CONF_MISRA_DIR)/misra.json # Check MISRA C compliancee if misra settings are added 
CHECKER_CMDS += --output-file=build/code-checking.log # Print the result in a log file 
CHECKER_CMDS += --error-exitcode=1 # Returns 1 if cppcheck has encountered an error

##############################################
############## CHECKER COMMANDS ##############
##############################################

CHECKER_ERROR_MSG = "\033[1;31mCode checked: errors have been found. Please correct them before continuing.\033[0m"

.PHONY += verif

verif :
	@mkdir -p $(BUILD_DIR)
	@$(CHECKER) $(CHECKER_CMDS) -D$(CHIP) -D$(BOARD) $(CHECKER_INCS) $(CHECKER_SRCS) || (cat build/code-checking.log; echo $(CHECKER_ERROR_MSG) ; exit 1)
	