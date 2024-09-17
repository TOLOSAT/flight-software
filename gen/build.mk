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
include gen/build_core.mk
include gen/build_application.mk
include gen/build_middlewares.mk
include gen/build_third_parties.mk
include gen/build_bsp.mk
include $(APPLICATION_DIR)/application.mk

##############################################
######## SOFTWARE BUILD CONFIGURATION ########
##############################################

PRIVATE_LIBS = $(foreach lib,$(PRIVATE_COMPONENTS),-l$(lib)-$(BUILD_TYPE))
PUBLIC_LIBS = $(foreach lib,$(PUBLIC_COMPONENTS),-l$(lib)-$(BUILD_TYPE))

##############################################
#################### BUILD ###################
##############################################

# Build recipes
.PHONY += build pre-build-info post-build-info build-clean
build : pre-build-info $(TARGET) post-build-info

# Display general build info before linking
pre-build-info :
	@echo "=============================="
	@echo "===    TAPAS BUILD INFO    ==="
	@echo "=============================="
	@echo "Software Version: $(VERSION)"
	@echo "Project Name: $(PROJ_NAME)"
	@echo "Compiler: $$( $(CC) --version | head -n 1 )"
	@echo "Build Type: $(BUILD_TYPE)"
	@echo "Board: $(BOARD)"
	@echo ""

# Target Linking Stage
$(TARGET) : pre-build $(PRIVATE_COMPONENTS) $(PUBLIC_COMPONENTS)
	@echo "=============================="
	@echo "===         LINKING        ==="
	@echo "=============================="
	@echo "  LD  $(@F)"
	@mkdir -p $(@D)
	@$(CC) -L$(LIBS_DIR) -Wl,--whole-archive $(PRIVATE_LIBS) -Wl,--no-whole-archive $(PUBLIC_LIBS) $(PROJECT_LDFLAGS) -T $(LD_SCRIPT) -o $@ > $(@:.elf=.size)
	@$(READELF) -a $@ > $(@:.elf=.readelf)
	@$(STRIP) $@ -o $(@D)/program.elf
	@$(PYTHON) $(TOOLS_DIR)/crc32-gen.py $(@D)/program.elf -o $(@D)/program.elf
	@echo "Linking Done"
	@echo ""

# Display post-build information and statistics
post-build-info :
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