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
include $(APPLICATIONS_DIR)/build.mk

##############################################
#################### BUILD ###################
##############################################

# Third parties (we need them as long as they are not comming inside kernel binary)
KERNEL_THIRD_PARTIES 	  = hal fatfs freertos
KERNEL_THIRD_PARTIES_LIBS = $(foreach lib,$(KERNEL_THIRD_PARTIES),-l$(lib))
LINK_LIBRARIES            = $(LIBS_DIR)/libapplications.a \
				            $(LIBS_DIR)/libkernel.a \
				            $(addprefix $(LIBS_DIR)/lib,$(addsuffix .a,$(APPLICATION_DEPENDANCIES))) \
				            $(addprefix $(LIBS_DIR)/lib,$(addsuffix .a,$(KERNEL_THIRD_PARTIES)))
LINK_FLAGS_FILE           = $(BUILD_STATE_DIR)/link.flags

# Build recipes
.PHONY : build build-dependencies build-end build-clean build-state-force
build :
	$(if $(PARALLEL_BUILD),$(QUIET_RECIPE),$(BUILD_START_VERBOSE))
	@$(MAKE) --no-print-directory build-dependencies
	@$(MAKE) --no-print-directory build-end
build-dependencies : kernel applications $(APPLICATION_DEPENDANCIES)
build-end : $(TARGET)
build-state-force :

$(LINK_FLAGS_FILE) : build-state-force
	@mkdir -p $(@D)
	@state_tmp="$@.tmp.$$$$"; \
	{ \
		printf '%s\n' 'compiler=$(CC)'; \
		printf '%s\n' 'compiler_version=$(CC_VERSION)'; \
		printf '%s\n' 'ldflags=$(PROJECT_LDFLAGS)'; \
		printf '%s\n' 'linker_script=$(LD_SCRIPT)'; \
		printf '%s\n' 'libraries=$(LINK_LIBRARIES)'; \
		printf '%s\n' 'application_libraries=$(APPLICATION_DEPENDANCIES_LIBS)'; \
		printf '%s\n' 'kernel_libraries=$(KERNEL_THIRD_PARTIES_LIBS)'; \
	} > "$$state_tmp"; \
	if cmp -s "$$state_tmp" "$@"; then rm -f "$$state_tmp"; else mv -f "$$state_tmp" "$@"; fi

define BUILD_START_VERBOSE
	@echo "$(BOLD)==============================$(RESET)"
	@echo "$(BOLD)===    TAPAS BUILD INFO    ===$(RESET)"
	@echo "$(BOLD)==============================$(RESET)"
	@echo "$(YELLOW)Software Version:$(RESET) v$(MAJOR).$(MINOR).$(PATCH)"
	@echo "$(YELLOW)Project Name:$(RESET) $(PROJ_NAME)"
	@echo "$(YELLOW)Compiler:$(RESET) $$( $(CC) --version | head -n 1 )"
	@echo "$(YELLOW)Board:$(RESET) $(BOARD)"
	@echo "$(YELLOW)Load Memory:$(RESET) $(LOAD_MEMORY)"
	@echo ""
endef

define LINK_START_VERBOSE
	@echo "$(BOLD)==============================$(RESET)"
	@echo "$(BOLD)===         LINKING        ===$(RESET)"
	@echo "$(BOLD)==============================$(RESET)"
	@echo "$(BLUE)Linking $(PROJ_NAME) executable...$(RESET)"
endef

define LINK_END_VERBOSE
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"
	@echo ""
endef

define BUILD_END_VERBOSE
	@echo "$(BOLD)==============================$(RESET)"
	@echo "$(BOLD)===    BUILD STATISTICS    ===$(RESET)"
	@echo "$(BOLD)==============================$(RESET)"
	@echo "$(YELLOW)Executable:$(RESET) $(TARGET)"
	@echo "$(YELLOW)Memory Usage:$(RESET)"
	@cat $(TARGET:.elf=.size)
	@echo "$(BOLD)$(GREEN)Build completed successfully!$(RESET)"
	@echo ""
endef

# Target Linking Stage
$(TARGET) : $(LINK_LIBRARIES) $(LD_SCRIPT) $(LINK_FLAGS_FILE)
	$(if $(PARALLEL_BUILD),$(QUIET_RECIPE),$(LINK_START_VERBOSE))
	@echo "  LD  [flight-software] $(@F)"
	@mkdir -p $(@D)
	@$(CC) -L$(LIBS_DIR) -Wl,--whole-archive -lapplications -lkernel $(APPLICATION_DEPENDANCIES_LIBS) -Wl,--no-whole-archive $(KERNEL_THIRD_PARTIES_LIBS) $(PROJECT_LDFLAGS) -T $(LD_SCRIPT) -o $@ > $(@:.elf=.size)
	@$(READELF) -a $@ > $(@:.elf=.readelf)
	@$(NM) -n -S -l $@ > $(@:.elf=.sym)
	@$(STRIP) $@ -o $(@D)/program.elf
	@$(PYTHON) $(TOOLS_DIR)/crc32-gen.py $(@D)/program.elf -o $(@D)/program.elf
	$(if $(PARALLEL_BUILD),$(QUIET_RECIPE),$(LINK_END_VERBOSE))

# Display post-build information and statistics
build-end :
	$(if $(PARALLEL_BUILD),$(QUIET_RECIPE),$(BUILD_END_VERBOSE))

# Clean recipe
build-clean :
	@printf "$(BLUE)Cleaning BUILD directory...$(RESET)"
	@rm -rf $(BUILD_DIR)
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"

endif # BUILD_BUILD_MK #
