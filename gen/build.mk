# Software Building Makefile

include gen/pre-build.mk
include gen/build_core.mk
include gen/build_application.mk
include gen/build_middlewares.mk
include gen/build_third_parties.mk
include gen/build_bsp.mk
include $(APPLICATION_DIR)/application.mk

##############################################
#################### BUILD ###################
##############################################

# Targets definitions
TARGET_DBG		= $(TARGET_DIR)/$(PROJ_NAME)-$(BUILD_TYPE).elf
TARGET_RLS		= $(TARGET_DIR)/$(PROJ_NAME)-$(BUILD_TYPE).elf

# Target definition according to version
ifeq ($(BUILD_TYPE), debug)
TARGET 			= $(TARGET_DBG)
VERSION_FLAGS 	= $(DEBUG_FLAGS)
else ifeq ($(BUILD_TYPE), release)
TARGET 			= $(TARGET_RLS)
VERSION_FLAGS 	= $(RELEASE_FLAGS)
else
$(error Please select debug or release)
endif

##############################################
######## SOFTWARE BUILD CONFIGURATION ########
##############################################

PRIVATE_LIBS = $(foreach lib,$(PRIVATE_COMPONENTS),-l$(lib)-$(BUILD_TYPE))
PUBLIC_LIBS = $(foreach lib,$(PUBLIC_COMPONENTS),-l$(lib)-$(BUILD_TYPE))

##############################################
################ BUILD RECIPE ################
##############################################

.PHONY += build

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
	@echo "Load Memory: $(LOAD_MEMORY)"
	@echo "Console Mode: $(CONSOLE_MODE)"
	@echo "File System Mode: $(FS_MODE)"
	@echo ""

# Target Linking Stage
$(TARGET) : pre-build $(PRIVATE_COMPONENTS) $(PUBLIC_COMPONENTS)
	@echo "=============================="
	@echo "===         LINKING        ==="
	@echo "=============================="
	@echo "  LD  $(@F)"
	@mkdir -p $(@D)
	@$(eval link_start_time=$(shell date +%s))
	@$(CC) -L$(BUILD_LIBS_DIR) -Wl,--whole-archive $(PRIVATE_LIBS) -Wl,--no-whole-archive $(PUBLIC_LIBS) $(PROJECT_LDFLAGS) -T $(LD_SCRIPT) -o $@ > $(@:.elf=.size)
	@$(eval link_end_time=$(shell date +%s))
	@$(READELF) -a $@ > $(@:.elf=.readelf)
	@$(STRIP) $@ -o $(@D)/program.elf
	@$(PYTHON) $(TOOLS_DIR)/crc32-gen.py $(@D)/program.elf -o $(@D)/program.elf
	@echo "Linking Done ($$(($(link_end_time)-$(link_start_time))) seconds elapsed)"
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