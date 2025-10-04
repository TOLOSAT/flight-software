# Makefile with verification rules

ifndef VERIFICATION_MK
VERIFICATION_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/paths.mk
include $(APPLICATIONS_DIR)/Makefile

##############################################
################ STATIC CHECK ################
##############################################

.PHONY : verif

# Main recipe
verif: kernel-verif $(foreach lib,$(APPLICATION_DEPENDANCIES),$(lib)-verif) applications-verif

##############################################
########## APPLICATION STATIC CHECK ##########
##############################################

# Application verif
.PHONY : applications-verif

CHECKER_ERROR_MSG = "\033[1;31mCode checked: errors have been found. Please correct them before continuing.\033[0m"

# Checker files and includes
CONF_MISRA = $(GEN_DIR)/MISRA/misra.json
CHECKER_LOGS = $(APPLICATIONS_OBJDIR)/code-checking.log

# Checker commands
CHECKER_CMDS  = --enable=all # Enables all warnings
CHECKER_CMDS += --suppress=missingInclude # Disables missing includes warnings
CHECKER_CMDS += --suppress=unusedFunction # Disables unused function warnings
CHECKER_CMDS += --inline-suppr # Allows to do suppress inside the code (inline)
CHECKER_CMDS += --addon=$(CONF_MISRA) # Check MISRA C compliancee if misra settings are added
CHECKER_CMDS += --output-file=$(CHECKER_LOGS) # Print the result in a log file
CHECKER_CMDS += --error-exitcode=1 # Returns 1 if cppcheck has encountered an error

# Checker recipes
applications-verif : conf-files
	@mkdir -p $(APPLICATIONS_OBJDIR)
	@$(CHECKER) $(CHECKER_CMDS) $(filter-out $(SYS_CONF_SRCS), $(APPLICATIONS_SRCS)) $(APPLICATIONS_INCFLAGS) $(CHECKER_SRCS) || (cat $(CHECKER_LOGS); echo $(CHECKER_ERROR_MSG) ; exit 1)

##############################################
############ DOCUMENTATION COMMAND ###########
##############################################

.PHONY : doc

doc :
	$(TOOLS_DIR)/update-doc.sh

##############################################
############### FORMAT COMMAND ###############
##############################################

.PHONY : format

format :
	$(TOOLS_DIR)/format-code.sh

endif # VERIFICATION_MK #