# Copyright (c) TOLOSAT 2026
# SPDX-License-Identifier: Apache-2.0

ifndef BUILD_CONFIGURATION_MK
BUILD_CONFIGURATION_MK := yes

KCONF_SCRIPT = $(GEN_DIR)/Kconfig
DEFCONFIG_FILE = $(CONFIGS_DIR)/$(CONFIG_NAME)_defconfig

.PHONY : config menuconfig savedefconfig

config :
	$(KCONF) conf $(KCONF_SCRIPT)

menuconfig :
	$(KCONF) mconf $(KCONF_SCRIPT)

%_defconfig:
	@printf "$(BLUE)Loading $@ for applications...$(RESET)"
	@cp $(CONFIGS_DIR)/$@ $(CONFIG_FILE)
	@rm -f $(OLD_CONFIG_FILE)
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"

savedefconfig:
	@printf "$(BLUE)Saving $(CONFIG_NAME)_defconfig...$(RESET)"
	@cp $(CONFIG_FILE) $(DEFCONFIG_FILE)
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"

endif # BUILD_CONFIGURATION_MK
