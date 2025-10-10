# Software Building Makefile

ifndef BUILD_BUILD_MK
BUILD_BUILD_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/paths.mk
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
	@echo "$(BOLD)==============================$(RESET)"
	@echo "$(BOLD)===    TAPAS BUILD INFO    ===$(RESET)"
	@echo "$(BOLD)==============================$(RESET)"
	@echo "$(YELLOW)Software Version:$(RESET) v$(MAJOR).$(MINOR).$(PATCH)"
	@echo "$(YELLOW)Project Name:$(RESET) $(PROJ_NAME)"
	@echo "$(YELLOW)Compiler:$(RESET) $$( $(CC) --version | head -n 1 )"
	@echo "$(YELLOW)Board:$(RESET) $(BOARD)"
	@echo "$(YELLOW)Load Memory:$(RESET) $(LOAD_MEMORY)"
ifneq ($(CONFIG_TEST_NAME),)
	@echo "$(YELLOW)Test:$(RESET) $(TEST_NAME)"
endif
	@echo ""

# Target Linking Stage
$(TARGET) : kernel pre-build applications $(APPLICATION_DEPENDANCIES)
	@echo "$(BOLD)==============================$(RESET)"
	@echo "$(BOLD)===         LINKING        ===$(RESET)"
	@echo "$(BOLD)==============================$(RESET)"
	@echo "$(BLUE)Linking $(PROJ_NAME) executable...$(RESET)"
	@echo "  LD  $(@F)"
	@mkdir -p $(@D)
	@$(CC) -L$(LIBS_DIR) -Wl,--whole-archive -lapplications -lkernel $(APPLICATION_DEPENDANCIES_LIBS) -Wl,--no-whole-archive $(KERNEL_THIRD_PARTIES_LIBS) $(PROJECT_LDFLAGS) -T $(LD_SCRIPT) -o $@ > $(@:.elf=.size)
	@$(READELF) -a $@ > $(@:.elf=.readelf)
	@$(NM) -n -S -l $@ > $(@:.elf=.sym)
	@$(STRIP) $@ -o $(@D)/program.elf
	@$(PYTHON) $(TOOLS_DIR)/crc32-gen.py $(@D)/program.elf -o $(@D)/program.elf
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"
	@echo ""

# Display post-build information and statistics
build-end :
	@echo "$(BOLD)==============================$(RESET)"
	@echo "$(BOLD)===    BUILD STATISTICS    ===$(RESET)"
	@echo "$(BOLD)==============================$(RESET)"
	@echo "$(YELLOW)Executable:$(RESET) $(TARGET)"
	@echo "$(YELLOW)Memory Usage:$(RESET)"
	@cat $(TARGET:.elf=.size)
	@echo "$(BOLD)$(GREEN)Build completed successfully!$(RESET)"
	@echo ""

# Clean recipe
build-clean :
	@printf "$(BLUE)Cleaning BUILD directory...$(RESET)"
	@rm -rf $(BUILD_DIR)
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"

endif # BUILD_BUILD_MK #