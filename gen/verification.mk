# Makefile with verification rules

ifndef BUILD_VERIFICATION_MK
BUILD_VERIFICATION_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk
include $(KERNEL_DIR)/Makefile
include $(APPLICATIONS_DIR)/Makefile

##############################################
################ CHECKER FILES ###############
##############################################

CONF_MISRA = $(GEN_DIR)/MISRA/misra.json

CHECKER_SRCS =	$(APPLICATIONS_SRCS) \
				$(KERNEL_SRCS) \
				$(wildcard $(PUS_DIR)/src/*.c $(PUS_DIR)/src/*/*.c) \
				$(wildcard $(IRIDIUM_DIR)/src/*.c $(IRIDIUM_DIR)/src/*/*.c)

CHECKER_SRCS := $(filter-out $(PRE_BUILD_DIR)/%, \
                $(filter-out $(KERNEL_SRCDIR)/bsp/%, $(CHECKER_SRCS)))

CHECKER_INCS =	$(APPLICATIONS_INCS) \
				-I$(KERNEL_INCLUDES) \
				-I$(KERNEL_INCDIR) \
				-I$(PRE_BUILD_DIR) \
				-I$(PUS_DIR)/inc \
				-I$(IRIDIUM_DIR)/inc \
				-I$(FREERTOS_CONFDIR) \
				-I$(FATFS_CONFDIR)

CHECKER_DEFS = -D$(CHIP) -D$(CHIP_FAMILLY) $(KERNEL_SELECT)

##############################################
############### CHECKER CONFIGS ##############
##############################################

CHECKER_CMDS  = --enable=all # Enables all warnings
CHECKER_CMDS += --suppress=missingInclude # Disables missing includes warnings
CHECKER_CMDS += --suppress=unusedFunction # Disables unused function warnings
CHECKER_CMDS += --inline-suppr # Allows to do suppress inside the code (inline)
CHECKER_CMDS += --addon=$(CONF_MISRA) # Check MISRA C compliancee if misra settings are added
CHECKER_CMDS += --output-file=build/code-checking.log # Print the result in a log file
CHECKER_CMDS += --error-exitcode=1 # Returns 1 if cppcheck has encountered an error
CHECKER_CMDS += --suppress=misra-c2012-11.5 # Suppression of this rule because its often use to pass parameters for callbacks inside the kernel

##############################################
############### CHECKER COMMAND ##############
##############################################

CHECKER_ERROR_MSG = "\033[1;31mCode checked: errors have been found. Please correct them before continuing.\033[0m"

.PHONY : verif

verif : autoconf conf-files
	@mkdir -p $(BUILD_DIR)
	@$(CHECKER) $(CHECKER_CMDS) $(CHECKER_DEFS) $(CHECKER_INCS) $(CHECKER_SRCS) || (cat build/code-checking.log; echo $(CHECKER_ERROR_MSG) ; exit 1)

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

endif # BUILD_VERIFICATION_MK #