# Makefile with verification rules

##############################################
################ CHECKER FILES ###############
##############################################

CHECKER_SRCS =	$(CORE_SRCS) \
				$(APPLICATION_SRCS) \
				$(LIBPUS_SRCS) \
				$(IRIDIUM_DRV_SRCS)

CHECKER_SRCS := $(filter-out $(PRE_BUILD_DIR)/%, $(CHECKER_SRCS))

CHECKER_INCS =	-I$(CORE_INCDIR) \
				-I$(APPLICATION_INCDIR) \
				-I$(PRE_BUILD_DIR) \
				-I$(LIBPUS_INCDIR) \
				-I$(IRIDIUM_DRV_INCDIR)

CHECKER_DEFS = -DSTM32H7

##############################################
############### CHECKER CONFIGS ##############
##############################################

CHECKER_CMDS  = --enable=all # Enables all warnings
CHECKER_CMDS += --suppress=missingInclude # Disables missing includes warnings
CHECKER_CMDS += --suppress=unusedFunction # Disables unused function warnings
CHECKER_CMDS += --inline-suppr # Allows to do suppress inside the code (inline)
CHECKER_CMDS += --addon=$(CONF_MISRA_DIR)/misra.json # Check MISRA C compliancee if misra settings are added 
CHECKER_CMDS += --output-file=build/code-checking.log # Print the result in a log file 
CHECKER_CMDS += --error-exitcode=1 # Returns 1 if cppcheck has encountered an error

CHECKER_CMDS += --suppress=misra-c2012-11.5

##############################################
############## CHECKER COMMANDS ##############
##############################################

CHECKER_ERROR_MSG = "\033[1;31mCode checked: errors have been found. Please correct them before continuing.\033[0m"

.PHONY += verif

verif :
	@mkdir -p $(BUILD_DIR)
	@$(CHECKER) $(CHECKER_CMDS) $(PROJECT_DEFINES) $(CHECKER_DEFS) $(CHECKER_INCS) $(CHECKER_SRCS) || (cat build/code-checking.log; echo $(CHECKER_ERROR_MSG) ; exit 1)
