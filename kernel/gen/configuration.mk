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

%_defconfig:
	@echo "Loading $@"
	@cp $(CONFIGS_DIR)/$@ $(CONFIG_FILE)
	@rm -f $(OLD_CONFIG_FILE)
	@rm -rf $(BUILD_DIR)
	@echo "Done"

check-config :
ifneq ($(CONFIG_CONFIG_NAME), "$(patsubst %_defconfig,%,$(TARGET_CONFIG))")
	@echo "*************************************************************"
	@echo "CONFIG_CONFIG_NAME ($(CONFIG_CONFIG_NAME)) does not match TARGET_CONFIG ($(patsubst %_defconfig,%,$(TARGET_CONFIG)))"
	@echo "Copying configs/$(TARGET_CONFIG) to $(CONFIG_FILE)"
	@echo "*************************************************************"
	@cp configs/$(TARGET_CONFIG) $(CONFIG_FILE)
endif

savedefconfig:
	@echo "Saving $(CONFIG_NAME)_defconfig"
	@cp $(CONFIG_FILE) $(DEFCONFIG_FILE)
	@echo "Done"

endif # BUILD_CONFIGURATION_MK #