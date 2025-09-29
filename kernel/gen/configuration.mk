# Configuration Makefile

ifndef BUILD_CONFIGURATION_MK
BUILD_CONFIGURATION_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk

##############################################
################ KCONFIG FILES ###############
##############################################

KCONF_SCRIPT	= gen/Kconfig
DEFCONFIG_FILE	= configs/$(CONFIG_NAME)_defconfig

##############################################
############### KCONFIG RECIPES ##############
##############################################

.PHONY : config menuconfig savedefconfig

config :
	$(KCONF) conf $(KCONF_SCRIPT)

menuconfig :
	$(KCONF) mconf $(KCONF_SCRIPT)

%_defconfig:
	@echo "Loading $@"
	@cp $(CONFIGS_DIR)/$@ $(CONFIG_FILE)
	@rm -f $(OLD_CONFIG_FILE)
	@rm -rf $(BUILD_DIR)
	@echo "Done"

savedefconfig:
	@echo "Saving $(CONFIG_NAME)_defconfig"
	@cp $(CONFIG_FILE) $(DEFCONFIG_FILE)
	@echo "Done"

endif # BUILD_CONFIGURATION_MK #