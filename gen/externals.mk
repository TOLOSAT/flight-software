# External build recipes

ifndef EXTERNALS_MK
EXTERNALS_MK := yes

##############################################
################### KERNEL ###################
##############################################

.PHONY : kernel kernel-%

kernel :
	@$(MAKE) -C $(KERNEL_DIR) \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		BUILD_DIR="$(BUILD_DIR)"

kernel-% :
	@$(MAKE) -C $(KERNEL_DIR) \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		BUILD_DIR="../$(BUILD_DIR)" \
		$(patsubst kernel-%,%,$@)

##############################################
################# PUS LIBRARY ################
##############################################

.PHONY : pus pus-%

pus:
	@$(MAKE) -C $(PUS_DIR) \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		KERNEL_HEADERS="$(KERNEL_HEADERS)" \
		BUILD_DIR="$(BUILD_DIR)"

pus-%:
	@$(MAKE) -C $(PUS_DIR) \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		KERNEL_HEADERS="$(KERNEL_HEADERS)" \
		BUILD_DIR="$(BUILD_DIR)" \
		$(patsubst pus-%,%,$@)

##############################################
############## IRIDIUM LIBRARY ###############
##############################################

.PHONY : iridium iridium-%

iridium:
	@$(MAKE) -C $(IRIDIUM_DIR) \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		KERNEL_HEADERS="$(KERNEL_HEADERS)" \
		EXTRA_INCS="$(PUS_DIR)/inc" \
		BUILD_DIR="$(BUILD_DIR)"

iridium-%:
	@$(MAKE) -C $(IRIDIUM_DIR) \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		KERNEL_HEADERS="$(KERNEL_HEADERS)" \
		EXTRA_INCS="$(PUS_DIR)/inc" \
		BUILD_DIR="$(BUILD_DIR)" \
		$(patsubst iridium-%,%,$@)

endif # EXTERNALS_MK #