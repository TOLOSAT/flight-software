# Makefile with verification rules

ifndef BUILD_VERIFICATION_MK
BUILD_VERIFICATION_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk
include gen/build_kernel.mk
include gen/build_application.mk
include gen/build_middlewares.mk

##############################################
################ CHECKER FILES ###############
##############################################

CHECKER_SRCS =	$(KERNEL_SRCS) \
				$(APPLICATION_SRCS) \
				$(PUS_SRCS) \
				$(IRIDIUMDRV_SRCS)

CHECKER_SRCS := $(filter-out $(PRE_BUILD_DIR)/%, $(CHECKER_SRCS))

CHECKER_INCS =	-I$(KERNEL_INCDIR) \
				-I$(PRE_BUILD_DIR) \
				-I$(APPLICATION_INCDIR) \
				-I$(PUS_INCDIR) \
				-I$(IRIDIUMDRV_INCDIR) \
				-I$(CONF_FREERTOS_DIR) \
				-I$(CONF_FATFS_DIR)

CHECKER_DEFS = -D$(CHIP) -D$(CHIP_FAMILLY) $(KERNEL_SELECT)

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

verif : autoconf conf-files
	@mkdir -p $(BUILD_DIR)
	@$(CHECKER) $(CHECKER_CMDS) $(CHECKER_DEFS) $(CHECKER_INCS) $(CHECKER_SRCS) || (cat build/code-checking.log; echo $(CHECKER_ERROR_MSG) ; exit 1)

endif # BUILD_VERIFICATION_MK #