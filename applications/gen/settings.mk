# Copyright (c) TOLOSAT 2026
# SPDX-License-Identifier: Apache-2.0

ifndef SETTINGS_MK
SETTINGS_MK := yes

KCONF = kconfig

CONFIG_FILE = .config
OLD_CONFIG_FILE = .config.old
DEFAULT_CONFIG = $(CONFIGS_DIR)/default_defconfig

ifneq ($(wildcard $(CONFIG_FILE)),)
include $(CONFIG_FILE)
else
include $(DEFAULT_CONFIG)
endif

CONFIG_NAME = $(subst ",,$(CONFIG_CONFIG_NAME))

endif # SETTINGS_MK
