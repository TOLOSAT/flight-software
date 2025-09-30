# Software Building Makefile

ifndef BUILD_BUILD_MK
BUILD_BUILD_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk
include gen/cc-settings.mk
include gen/pre-build.mk
include gen/externals.mk
include $(APPLICATIONS_DIR)/Makefile

##############################################
#################### BUILD ###################
##############################################

# Third parties (we need them as long as they are not comming inside kernel binary)
KERNEL_THIRD_PARTIES 	= hal fatfs freertos
KERNEL_THIRD_PARTIES_LIBS = $(foreach lib,$(KERNEL_THIRD_PARTIES),-l$(lib))

# Build recipes
.PHONY : build build-start build-end build-clean
build : build-start $(TARGET) build-end

# Display general build info before linking
build-start :
	@echo "=============================="
	@echo "===    TAPAS BUILD INFO    ==="
	@echo "=============================="
	@echo "Software Version: v$(MAJOR).$(MINOR).$(PATCH)"
	@echo "Project Name: $(PROJ_NAME)"
	@echo "Compiler: $$( $(CC) --version | head -n 1 )"
	@echo "Board: $(BOARD)"
	@echo "Load Memory: $(LOAD_MEMORY)"
	@echo "Test : $(TEST_NAME)"
	@echo ""

# Target Linking Stage
$(TARGET) : kernel pre-build applications $(APPLICATION_DEPENDANCIES)
	@echo "=============================="
	@echo "===         LINKING        ==="
	@echo "=============================="
	@echo "  LD  $(@F)"
	@mkdir -p $(@D)
	@$(CC) -L$(LIBS_DIR) -Wl,--whole-archive -lapplications -lkernel $(APPLICATION_DEPENDANCIES_LIBS) -Wl,--no-whole-archive $(KERNEL_THIRD_PARTIES_LIBS) $(PROJECT_LDFLAGS) -T $(LD_SCRIPT) -o $@ > $(@:.elf=.size)
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