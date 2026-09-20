# Applications Building Makefile

ifndef BUILD_APPLICATIONS_MK
BUILD_APPLICATIONS_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/paths.mk
include gen/cc-settings.mk

##############################################
################ CONFIGURATION ###############
##############################################

APPLICATION_COMPONENTS = aocs gravimetry iridium power thermal tmtc
APPLICATION_DEPENDANCIES = iridium thermal pus

##############################################
########## APPLICATIONS DIRECTORIES ##########
##############################################

# Applications Directories
APPLICATIONS_COMPONENTS_DIR	= $(APPLICATIONS_DIR)/components
APPLICATIONS_CONF_DIR		= $(APPLICATIONS_DIR)/conf
APPLICATIONS_OBJDIR			= $(BUILD_DIR)/applications

# Dependancies
APPLICATION_DEPENDANCIES_INCDIRS = $(addprefix $(MIDDLEWARES_DIR)/, $(addsuffix -library/include,$(APPLICATION_DEPENDANCIES)))
APPLICATION_DEPENDANCIES_LIBS    = $(foreach lib,$(APPLICATION_DEPENDANCIES),-l$(lib))

##############################################
################ APPLICATIONS ################
##############################################

# Applications files
APPLICATIONS_SRCS = $(foreach component,$(APPLICATION_COMPONENTS),$(wildcard $(APPLICATIONS_COMPONENTS_DIR)/$(component)/src/*.c)) $(SYS_CONF_SRCS)
APPLICATIONS_OBJS = $(foreach component,$(APPLICATION_COMPONENTS), \
				    $(patsubst $(APPLICATIONS_COMPONENTS_DIR)/$(component)/src/%.c,$(APPLICATIONS_OBJDIR)/$(component)/%.o, \
				    $(filter $(APPLICATIONS_COMPONENTS_DIR)/$(component)/src/%.c,$(APPLICATIONS_SRCS)))) \
				    $(patsubst $(PRE_BUILD_DIR)/%.c,$(APPLICATIONS_OBJDIR)/conf/%.o,$(SYS_CONF_SRCS))
APPLICATIONS_LIB = $(LIBS_DIR)/libapplications.a
APPLICATIONS_FLAGS_FILE = $(BUILD_STATE_DIR)/applications.flags

# Applications flags
APPLICATIONS_CFLAGS   = $(PROJECT_CFLAGS)
APPLICATIONS_INCDIRS  = $(APPLICATIONS_COMPONENTS_DIR) $(APPLICATION_DEPENDANCIES_INCDIRS) \
						$(KERNEL_HEADERS) $(PRE_BUILD_DIR) \
						$(BUILD_DIR)/kernel/conf # TO DO : DEFINE A KERNEL CONF USER INTERFACE
APPLICATIONS_PRIVATE_INCDIRS = $(foreach component,$(APPLICATION_COMPONENTS),$(wildcard $(APPLICATIONS_COMPONENTS_DIR)/$(component)/inc))
APPLICATIONS_INCFLAGS = $(addprefix -I,$(APPLICATIONS_INCDIRS))
APPLICATIONS_CHECKER_INCFLAGS = $(APPLICATIONS_INCFLAGS) $(addprefix -I,$(APPLICATIONS_PRIVATE_INCDIRS))

# Include dependencies
-include $(APPLICATIONS_OBJS:.o=.d)

# Applications recipes
.PHONY : applications applications-start applications-end applications-clean build-state-force
applications : pre-build kernel-pre-build
	@$(MAKE) --no-print-directory applications-end
applications-end : $(APPLICATIONS_LIB)
$(APPLICATIONS_OBJS) : | applications-start
build-state-force :

$(APPLICATIONS_FLAGS_FILE) : build-state-force
	@mkdir -p $(@D)
	@state_tmp="$@.tmp.$$$$"; \
	{ \
		printf '%s\n' 'compiler=$(CC)'; \
		printf '%s\n' 'compiler_version=$(CC_VERSION)'; \
		printf '%s\n' 'archiver=$(AR)'; \
		printf '%s\n' 'cflags=$(APPLICATIONS_CFLAGS)'; \
		printf '%s\n' 'include_dirs=$(APPLICATIONS_INCDIRS)'; \
		printf '%s\n' 'private_include_dirs=$(APPLICATIONS_PRIVATE_INCDIRS)'; \
		printf '%s\n' 'sources=$(APPLICATIONS_SRCS)'; \
	} > "$$state_tmp"; \
	if cmp -s "$$state_tmp" "$@"; then rm -f "$$state_tmp"; else mv -f "$$state_tmp" "$@"; fi

define APPLICATIONS_START_VERBOSE
	@echo "$(BOLD)=============================$(RESET)"
	@echo "$(BOLD)===      APPLICATIONS     ===$(RESET)"
	@echo "$(BOLD)=============================$(RESET)"
	@echo "$(YELLOW)Files to compile:$(RESET) $(words $(APPLICATIONS_SRCS))"
	@echo "$(YELLOW)Compilation Flags:$(RESET)"
	@echo $(APPLICATIONS_CFLAGS)
	@echo "$(YELLOW)Include Paths:$(RESET)"
	@$(foreach dir,$(patsubst $(WORKSPACE)/%,%,$(APPLICATIONS_INCDIRS)),echo "  - $(dir)";)
	@$(foreach dir,$(patsubst $(WORKSPACE)/%,%,$(APPLICATIONS_PRIVATE_INCDIRS)),echo "  - $(dir) (private)";)
	@echo "$(BLUE)Start building...$(RESET)"
endef

define APPLICATIONS_END_VERBOSE
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"
	@echo ""
endef

# Build header
applications-start :
	$(if $(PARALLEL_BUILD),$(QUIET_RECIPE),$(APPLICATIONS_START_VERBOSE))

# Building recipes
define APPLICATION_COMPONENT_RULE
$(APPLICATIONS_OBJDIR)/$(1)/%.o : $(APPLICATIONS_COMPONENTS_DIR)/$(1)/src/%.c $(APPLICATIONS_FLAGS_FILE)
	@echo "  CC  [applications/$(1)] $$(@F)"
	@mkdir -p $$(@D)
	@$(CC) $(APPLICATIONS_CFLAGS) $(APPLICATIONS_INCFLAGS) $(if $(wildcard $(APPLICATIONS_COMPONENTS_DIR)/$(1)/inc),-iquote $(APPLICATIONS_COMPONENTS_DIR)/$(1)/inc) $$< -o $$@
endef

$(foreach component,$(APPLICATION_COMPONENTS),$(eval $(call APPLICATION_COMPONENT_RULE,$(component))))

$(PRE_BUILD_DIR)/%.o : $(PRE_BUILD_DIR)/%.c $(APPLICATIONS_FLAGS_FILE)
	@echo "  CC  [applications/generated] $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(APPLICATIONS_CFLAGS) $(APPLICATIONS_INCFLAGS) $< -o $@

# Library generation
$(APPLICATIONS_LIB) : $(APPLICATIONS_OBJS) $(APPLICATIONS_FLAGS_FILE)
	@echo "  AR  [applications] $(@F)"
	@mkdir -p $(@D)
	@rm -f $@
	@$(AR) rcs $@ $(APPLICATIONS_OBJS)

# Build footer
applications-end :
	$(if $(PARALLEL_BUILD),$(QUIET_RECIPE),$(APPLICATIONS_END_VERBOSE))

# Clean recipe
applications-clean :
	@printf "$(BLUE)Cleaning APPLICATIONS build directory...$(RESET)"
	@rm -rf $(APPLICATIONS_OBJDIR)
	@rm -rf $(APPLICATIONS_LIB)
	@rm -f $(APPLICATIONS_FLAGS_FILE)
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"

endif # BUILD_APPLICATIONS_MK #
