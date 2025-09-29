# External build recipes

ifndef EXTERNALS_MK
EXTERNALS_MK := yes

# Iridium Library

iridium: pre-build
	@make -C $(IRIDIUM_DIR) \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		KERNEL_HEADERS="../../$(KERNEL_INCLUDES)" \
		EXTRA_INCS="../../$(PUS_DIR)/inc" \
		BUILD_DIR="../../$(BUILD_DIR)"

iridium-clean:
	@make -C $(IRIDIUM_DIR) clean \
		BUILD_DIR="../../$(BUILD_DIR)"

# PUS Library

pus: pre-build
	@make -C $(PUS_DIR) \
		TOOLCHAIN="$(TOOLCHAIN)" \
		CFLAGS="$(PROJECT_CFLAGS)" \
		KERNEL_HEADERS="../../$(KERNEL_INCLUDES)" \
		EXTRA_INCS="../../$(PRE_BUILD_DIR)" \
		BUILD_DIR="../../$(BUILD_DIR)"

pus-clean:
	@make -C $(PUS_DIR) clean \
		BUILD_DIR="../../$(BUILD_DIR)"

endif # EXTERNALS_MK #