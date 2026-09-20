# Pre Building Makefile

ifndef PRE_BUILD_MK
PRE_BUILD_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/paths.mk

##############################################
################## PRE-BUILD #################
##############################################

# JSON configuration file
CONF_JSON = $(APPLICATIONS_DIR)/system.json

# List of generated configuration files
SYS_CONF_SRCS = $(PRE_BUILD_DIR)/tasks_conf.c \
				$(PRE_BUILD_DIR)/buffers_conf.c \
				$(PRE_BUILD_DIR)/mutex_conf.c \
				$(PRE_BUILD_DIR)/fs_conf.c \
				$(PRE_BUILD_DIR)/timers_conf.c \
				$(PRE_BUILD_DIR)/callbacks_conf.c
SYS_CONF_HEADERS = $(PRE_BUILD_DIR)/tasks_conf.h \
				   $(PRE_BUILD_DIR)/buffers_conf.h \
				   $(PRE_BUILD_DIR)/mutex_conf.h \
				   $(PRE_BUILD_DIR)/fs_conf.h \
				   $(PRE_BUILD_DIR)/timers_conf.h \
				   $(PRE_BUILD_DIR)/system_conf.h
SYS_CONF_FILES = $(SYS_CONF_SRCS) $(SYS_CONF_HEADERS)
SYSTEM_CONF_STAMP = $(PRE_BUILD_DIR)/system-conf.stamp
SYSTEM_CONF_GENERATOR = $(TOOLS_DIR)/system-parser.py

# A stamp alone cannot detect a manually removed generated file. Force the
# generator once when at least one declared output is missing.
SYSTEM_CONF_MISSING := $(filter-out $(wildcard $(SYS_CONF_FILES)),$(SYS_CONF_FILES))
ifneq ($(SYSTEM_CONF_MISSING),)
.PHONY : system-conf-missing
$(SYSTEM_CONF_STAMP) : system-conf-missing
endif

# System configuration recipes
.PHONY : pre-build pre-build-start conf-files pre-build-end pre-build-clean
pre-build : pre-build-end
pre-build-end : conf-files
conf-files : | pre-build-start

define PRE_BUILD_START_VERBOSE
	@echo "$(BOLD)=============================$(RESET)"
	@echo "$(BOLD)===       PRE BUILD       ===$(RESET)"
	@echo "$(BOLD)=============================$(RESET)"
	@echo "$(YELLOW)Files to pre-build:$(RESET) $(words $(SYS_CONF_FILES))"
	@echo "$(BLUE)Start pre-building...$(RESET)"
endef

define PRE_BUILD_END_VERBOSE
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"
	@echo ""
endef

# System configuration header
pre-build-start :
	$(if $(PARALLEL_BUILD),$(QUIET_RECIPE),$(PRE_BUILD_START_VERBOSE))

# System configuration files generation
conf-files : $(SYS_CONF_FILES)

$(SYS_CONF_FILES) : | $(SYSTEM_CONF_STAMP)

$(SYSTEM_CONF_STAMP) : $(CONF_JSON) $(SYSTEM_CONF_GENERATOR)
	@mkdir -p $(@D)
	@echo "  PY  [applications/generated] tasks_conf.c, tasks_conf.h"
	@echo "  PY  [applications/generated] buffers_conf.c, buffers_conf.h"
	@echo "  PY  [applications/generated] mutex_conf.c, mutex_conf.h"
	@echo "  PY  [applications/generated] fs_conf.c, fs_conf.h"
	@echo "  PY  [applications/generated] timers_conf.c, timers_conf.h"
	@echo "  PY  [applications/generated] callbacks_conf.c"
	@echo "  PY  [applications/generated] system_conf.h"
	@${PYTHON} $(SYSTEM_CONF_GENERATOR) -i $(CONF_JSON) -o $(PRE_BUILD_DIR)
	@stamp_tmp="$@.tmp.$$$$"; \
		printf '%s\n' $(notdir $(SYS_CONF_FILES)) > "$$stamp_tmp"; \
		mv -f "$$stamp_tmp" "$@"

# System configuration footer
pre-build-end :
	$(if $(PARALLEL_BUILD),$(QUIET_RECIPE),$(PRE_BUILD_END_VERBOSE))

# System configuration clean recipes
pre-build-clean :
	@echo "$(BLUE)Cleaning APP-CONF build directory...$(RESET)"
	@rm -rf $(PRE_BUILD_DIR)
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"

endif # PRE_BUILD_MK #
