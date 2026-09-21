# Copyright (c) TOLOSAT 2026
# SPDX-License-Identifier: Apache-2.0

# Applications configuration Makefile

.DEFAULT_GOAL := help

.PHONY : help clean

include gen/paths.mk
include gen/settings.mk
include gen/configuration.mk

help :
	@echo "Usage: make <target>"
	@echo ""
	@echo "Configuration targets:"
	@echo "  make default_defconfig"
	@echo "  make config"
	@echo "  make menuconfig"
	@echo "  make savedefconfig"

clean :
	@rm -f $(CONFIG_FILE) $(OLD_CONFIG_FILE)

print-%:
	@printf '%s = %s\n' '$*' '$($*)'

GREEN = \033[92m
BLUE = \033[94m
BOLD = \033[1m
RESET = \033[0m
