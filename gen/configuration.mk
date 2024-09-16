# Configuration Makefile

ifndef BUILD_CONFIGURATION_MK
BUILD_CONFIGURATION_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk

##############################################
############### KCONFIG RECIPES ##############
##############################################

config :
	$(KCONF) conf $(KCONF_SCRIPT)

menuconfig :
	$(KCONF) mconf $(KCONF_SCRIPT)

%_defconfig:
	@echo "Loading $@"
	@cp $(CONFIGS_DIR)/$@ $(CONFIG_FILE)
	@rm -f $(OLD_CONFIG_FILE)

savedefconfig:
	@echo "Saving $(shell grep CONFIG_NAME .config | cut -d'=' -f2 | tr -d '"')_defconfig"
	@cp $(CONFIG_FILE) $(CONFIGS_DIR)/$(shell grep CONFIG_NAME .config | cut -d'=' -f2 | tr -d '"')_defconfig

endif # BUILD_CONFIGURATION_MK #