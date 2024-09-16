# Configuration Makefile

##############################################
############### KCONFIG RECIPES ##############
##############################################

config :
	$(KCONF) conf $(KCONF_SCRIPT)

menuconfig :
	$(KCONF) mconf $(KCONF_SCRIPT)

%_defconfig:
	@echo "Loading $@"
	@cp $(CONFIGS_DIR)/$@ .config
	@rm -f .config.old

savedefconfig:
	@echo "Saving $(shell grep CONFIG_NAME .config | cut -d'=' -f2 | tr -d '"')_defconfig"
	@cp .config $(CONFIGS_DIR)/$(shell grep CONFIG_NAME .config | cut -d'=' -f2 | tr -d '"')_defconfig
