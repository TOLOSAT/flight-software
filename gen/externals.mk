# External build recipes

ifndef EXTERNALS_MK
EXTERNALS_MK := yes

##############################################
################### KERNEL ###################
##############################################

.PHONY : kernel kernel-%

kernel :
	@$(MAKE) -C $(KERNEL_DIR) \
		WORKSPACE="$(WORKSPACE)" \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		TARGET_CONFIG="$(CONFIG_KERNEL_DEFCONFIG)"

kernel-% :
	@$(MAKE) -C $(KERNEL_DIR) \
		WORKSPACE="$(WORKSPACE)" \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		TARGET_CONFIG="$(CONFIG_KERNEL_DEFCONFIG)" \
		$(patsubst kernel-%,%,$@)

kernel-%_defconfig:
	@$(MAKE) -C $(KERNEL_DIR) \
		WORKSPACE="$(WORKSPACE)" \
		$(patsubst kernel-%,%,$@)

##############################################
################# PUS LIBRARY ################
##############################################

# Directory
PUS_DIR = $(MIDDLEWARES_DIR)/pus-library

# Recipes
.PHONY : pus pus-%

pus:
	@$(MAKE) -C $(PUS_DIR) \
		WORKSPACE="$(WORKSPACE)" \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		KERNEL_HEADERS="$(KERNEL_HEADERS)"

pus-%:
	@$(MAKE) -C $(PUS_DIR) \
		WORKSPACE="$(WORKSPACE)" \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		KERNEL_HEADERS="$(KERNEL_HEADERS)"
		$(patsubst pus-%,%,$@)

##############################################
############## IRIDIUM LIBRARY ###############
##############################################

# Directory
IRIDIUM_DIR = $(MIDDLEWARES_DIR)/iridium-library

# Recipes
.PHONY : iridium iridium-%

iridium :
	@$(MAKE) -C $(IRIDIUM_DIR) \
		WORKSPACE="$(WORKSPACE)" \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		KERNEL_HEADERS="$(KERNEL_HEADERS)" \
		EXTRA_INCS="$(PUS_DIR)/inc"

iridium-% :
	@$(MAKE) -C $(IRIDIUM_DIR) \
		WORKSPACE="$(WORKSPACE)" \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		KERNEL_HEADERS="$(KERNEL_HEADERS)" \
		EXTRA_INCS="$(PUS_DIR)/inc"
		$(patsubst iridium-%,%,$@)

endif # EXTERNALS_MK #