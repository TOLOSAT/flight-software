# External build recipes

ifndef EXTERNALS_MK
EXTERNALS_MK := yes

##############################################
################### KERNEL ###################
##############################################

.PHONY : kernel kernel-%

kernel : kernel-pre-build
	@$(MAKE) --no-print-directory \
		-C $(KERNEL_DIR) \
		WORKSPACE="$(WORKSPACE)" \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		hal fatfs freertos kernel

kernel-% :
	@$(MAKE) --no-print-directory \
		-C $(KERNEL_DIR) \
		WORKSPACE="$(WORKSPACE)" \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		$(patsubst kernel-%,%,$@)

kernel-%_defconfig:
	@$(MAKE) --no-print-directory \
		-C $(KERNEL_DIR) \
		WORKSPACE="$(WORKSPACE)" \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		$(patsubst kernel-%,%,$@)

kernel-force-defconfig:
	@echo "$(YELLOW)Forcing kernel to load $(CONFIG_KERNEL_DEFCONFIG)...$(RESET)"
	@$(MAKE) --no-print-directory \
		-C $(KERNEL_DIR) \
		WORKSPACE="$(WORKSPACE)" \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		$(CONFIG_KERNEL_DEFCONFIG)

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
		EXTRA_INCS="$(PUS_DIR)/inc"

iridium-% :
	@$(MAKE) --no-print-directory \
		-C $(IRIDIUM_DIR) \
		WORKSPACE="$(WORKSPACE)" \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		KERNEL_HEADERS="$(KERNEL_HEADERS)" \
		EXTRA_INCS="$(PUS_DIR)/inc" \
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
		EXTRA_INCS="$(PUS_DIR)/inc"

thermal-% :
	@$(MAKE) --no-print-directory \
		-C $(THERMAL_DIR) \
		WORKSPACE="$(WORKSPACE)" \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		KERNEL_HEADERS="$(KERNEL_HEADERS)" \
		EXTRA_INCS="$(PUS_DIR)/inc" \
		$(patsubst thermal-%,%,$@)

endif # EXTERNALS_MK #
