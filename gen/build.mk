# Software Building Makefile

ifndef BUILD_BUILD_MK
BUILD_BUILD_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk
include gen/cc_settings.mk
include gen/pre_build.mk
include $(KERNEL_DIR)/Makefile
include $(APPLICATIONS_DIR)/Makefile

##############################################
#################### BUILD ###################
##############################################

# Build recipes
.PHONY : build build-start build-end build-clean
build : build-start $(TARGET) build-end

# External Makefiles
iridium:
	@make -C $(IRIDIUM_DIR) \
		BUILD_DIR="$(BUILD_DIR)" \
		CC="$(CC)" \
		CFLAGS="$(PROJECT_CFLAGS) $(VERSION_FLAGS)" \
		KERNEL_HEADERS="$(KERNEL_INCLUDES)" \
		PRE_BUILD_HEADERS="$(PRE_BUILD_DIR)" \
		PUS_HEADERS="$(PUS_DIR)/inc"

iridium-clean:
	@make -C $(IRIDIUM_DIR) clean \
		BUILD_DIR="$(BUILD_DIR)"

pus:
	@make -C $(PUS_DIR) \
		BUILD_DIR="$(BUILD_DIR)" \
		CC="$(CC)" \
		CFLAGS="$(PROJECT_CFLAGS) $(VERSION_FLAGS)" \
		KERNEL_HEADERS="$(KERNEL_INCLUDES)" \
		PRE_BUILD_HEADERS="$(PRE_BUILD_DIR)"

pus-clean:
	@make -C $(PUS_DIR) clean \
		BUILD_DIR="$(BUILD_DIR)"

# Display general build info before linking
build-start :
	@echo "=============================="
	@echo "===    TAPAS BUILD INFO    ==="
	@echo "=============================="
	@echo "Software Version: v$(MAJOR).$(MINOR).$(PATCH)"
	@echo "Project Name: $(PROJ_NAME)"
	@echo "Compiler: $$( $(CC) --version | head -n 1 )"
	@echo "Build Type: $(BUILD_TYPE)"
	@echo "Board: $(BOARD)"
	@echo "Load Memory: $(LOAD_MEMORY)"
	@echo "Test : $(TEST_NAME)"
	@echo ""

# Target Linking Stage
$(TARGET) : pre-build applications kernel $(APPLICATION_DEPENDANCIES) $(KERNEL_THIRD_PARTIES)
	@echo "=============================="
	@echo "===         LINKING        ==="
	@echo "=============================="
	@echo "  LD  $(@F)"
	@mkdir -p $(@D)
	@$(CC) -L$(LIBS_DIR) -Wl,--whole-archive -lapplications-$(BUILD_TYPE) -lkernel-$(BUILD_TYPE) $(APPLICATION_DEPENDANCIES_LIBS) -Wl,--no-whole-archive $(KERNEL_THIRD_PARTIES_LIBS) $(PROJECT_LDFLAGS) -T $(LD_SCRIPT) -o $@ > $(@:.elf=.size)
	@$(READELF) -a $@ > $(@:.elf=.readelf)
	@$(NM) -n -S -l $@ > $(@:.elf=.sym)
	@$(STRIP) $@ -o $(@D)/program.elf
	@$(PYTHON) $(TOOLS_DIR)/crc32-gen.py $(@D)/program.elf -o $(@D)/program.elf
	@echo "Linking Done"
	@echo ""

# Display post-build information and statistics
build-end :
	@echo "=============================="
	@echo "===    BUILD STATISTICS    ==="
	@echo "=============================="
	@echo "Executable: $(TARGET)"
	@echo "Memory Usage :"
	@cat $(TARGET:.elf=.size)
	@echo "Build completed successfully."
	@echo ""

# Clean recipe
build-clean :
	@echo "Cleaning BUILD directory ..."
	@rm -rf $(BUILD_DIR)
	@echo "Done"

endif # BUILD_BUILD_MK #