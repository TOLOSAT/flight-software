# Copyright (c) TOLOSAT 2026
# SPDX-License-Identifier: Apache-2.0

# External build recipes

ifndef EXTERNALS_MK
EXTERNALS_MK := yes

##############################################
################### KERNEL ###################
##############################################

.PHONY : kernel

kernel : kernel-pre-build
	@$(MAKE) --no-print-directory \
		-C $(KERNEL_DIR) \
		WORKSPACE="$(WORKSPACE)" \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		CONFIG_FILE="$(abspath $(CONFIG_FILE))" \
		KERNEL_PRE_BUILD_DONE=1 \
		hal fatfs freertos kernel

kernel-% :
	@$(MAKE) --no-print-directory \
		-C $(KERNEL_DIR) \
		WORKSPACE="$(WORKSPACE)" \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		CONFIG_FILE="$(abspath $(CONFIG_FILE))" \
		$(patsubst kernel-%,%,$@)

##############################################
################# PUS LIBRARY ################
##############################################

# Directory
PUS_DIR = $(MIDDLEWARES_DIR)/pus-library

# Recipes
.PHONY : pus pus-%

pus:
	@$(MAKE) --no-print-directory \
		-C $(PUS_DIR) \
		WORKSPACE="$(WORKSPACE)" \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		KERNEL_HEADERS="$(KERNEL_HEADERS)"

pus-%:
	@$(MAKE) --no-print-directory \
		-C $(PUS_DIR) \
		WORKSPACE="$(WORKSPACE)" \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		KERNEL_HEADERS="$(KERNEL_HEADERS)" \
		$(patsubst pus-%,%,$@)

##############################################
############## IRIDIUM LIBRARY ###############
##############################################

# Directory
IRIDIUM_DIR = $(MIDDLEWARES_DIR)/iridium-library

# Recipes
.PHONY : iridium iridium-%

iridium :
	@$(MAKE) --no-print-directory \
		 -C $(IRIDIUM_DIR) \
		WORKSPACE="$(WORKSPACE)" \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		KERNEL_HEADERS="$(KERNEL_HEADERS)" \
		EXTRA_INCS="$(PUS_DIR)/include"

iridium-% :
	@$(MAKE) --no-print-directory \
		-C $(IRIDIUM_DIR) \
		WORKSPACE="$(WORKSPACE)" \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		KERNEL_HEADERS="$(KERNEL_HEADERS)" \
		EXTRA_INCS="$(PUS_DIR)/include" \
		$(patsubst iridium-%,%,$@)

##############################################
############## THERMAL LIBRARY ###############
##############################################

# Directory
THERMAL_DIR = $(MIDDLEWARES_DIR)/thermal-library

# Recipes
.PHONY : thermal thermal-%

thermal :
	@$(MAKE) --no-print-directory \
		 -C $(THERMAL_DIR) \
		WORKSPACE="$(WORKSPACE)" \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		KERNEL_HEADERS="$(KERNEL_HEADERS)" \
		EXTRA_INCS="$(PUS_DIR)/include"

thermal-% :
	@$(MAKE) --no-print-directory \
		-C $(THERMAL_DIR) \
		WORKSPACE="$(WORKSPACE)" \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		KERNEL_HEADERS="$(KERNEL_HEADERS)" \
		EXTRA_INCS="$(PUS_DIR)/include" \
		$(patsubst thermal-%,%,$@)

endif # EXTERNALS_MK #
