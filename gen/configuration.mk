# Configuration Makefile

ifndef BUILD_CONFIGURATION_MK
BUILD_CONFIGURATION_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/paths.mk

##############################################
################ KCONFIG FILES ###############
##############################################

KCONF_SCRIPT	= $(GEN_DIR)/Kconfig
DEFCONFIG_FILE	= $(CONFIGS_DIR)/$(CONFIG_NAME)_defconfig

##############################################
############### KCONFIG RECIPES ##############
##############################################

.PHONY : config menuconfig savedefconfig

config :
	$(KCONF) conf $(KCONF_SCRIPT)

menuconfig :
	$(KCONF) mconf $(KCONF_SCRIPT)

# Prefixed configuration commands launched from the parent repository edit the
# integrated firmware configuration. Standalone commands remain available from
# each component's working directory.
%-config : config
	@:

%-menuconfig : menuconfig
	@:

%-savedefconfig : savedefconfig
	@:

%_defconfig:
	@printf "$(BLUE)Loading $@ for $(PROJ_NAME)...$(RESET)"
	@cp $(CONFIGS_DIR)/$@ $(CONFIG_FILE)
	@rm -f $(OLD_CONFIG_FILE)
	@rm -rf $(BUILD_DIR)
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"

savedefconfig:
	@printf "$(BLUE)Saving $(CONFIG_NAME)_defconfig$(RESET)"
	@cp $(CONFIG_FILE) $(DEFCONFIG_FILE)
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"

endif # BUILD_CONFIGURATION_MK #