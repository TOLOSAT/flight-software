# Pre Building Makefile

ifndef BUILD_PRE_BUILD_MK
BUILD_PRE_BUILD_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk

##############################################
################## PRE-BUILD #################
##############################################

# CSV conf files
CONF_CSV = $(wildcard $(APPLICATIONS_CONF_DIR)/*.csv)
CONF_SRCS = $(subst $(APPLICATIONS_CONF_DIR)/,$(PRE_BUILD_DIR)/conf/,$(CONF_CSV:.csv=.c))

# Autoconf file
AUTOCONF_SRC = $(PRE_BUILD_DIR)/autoconf.h

# Pre-build recipes
.PHONY += pre-build pre-build-start autoconf conf-files linker-script pre-build-end pre-build-clean
pre-build : pre-build-start autoconf conf-files linker-script pre-build-end

# Pre-build header
pre-build-start :
	@echo "============================="
	@echo "===       PRE BUILD       ==="
	@echo "============================="
	@echo "Files to pre-build: $(words $(CONF_SRCS) $(RAW_LD_SCRIPT) $(AUTOCONF_SRC))"
	@echo "Start pre-building:"

# Autoconf recipes
autoconf : $(AUTOCONF_SRC)

$(AUTOCONF_SRC) : $(CONFIG_FILE)
	@echo "  PY  $(@F)"
	@${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/config-parser.py -i $^ -o $(@D)

# Configuration files recipes
conf-files : $(CONF_SRCS)

$(PRE_BUILD_DIR)/conf/tasks_conf.c : $(APPLICATIONS_CONF_DIR)/tasks_conf.csv
	@echo "  PY  $(@F), $(@F:.c=.h)"
	@${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/tasks-parser.py -i $^ -o $(@D)

$(PRE_BUILD_DIR)/conf/buffers_conf.c : $(APPLICATIONS_CONF_DIR)/buffers_conf.csv
	@echo "  PY  $(@F), $(@F:.c=.h)"
	@${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/buffers-parser.py -i $^ -o $(@D)

$(PRE_BUILD_DIR)/conf/mutex_conf.c : $(APPLICATIONS_CONF_DIR)/mutex_conf.csv
	@echo "  PY  $(@F), $(@F:.c=.h)"
	@${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/mutex-parser.py -i $^ -o $(@D)

$(PRE_BUILD_DIR)/conf/fs_conf.c : $(APPLICATIONS_CONF_DIR)/fs_conf.csv
	@echo "  PY  $(@F), $(@F:.c=.h)"
	@${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/fs-parser.py -i $^ -o $(@D)

$(PRE_BUILD_DIR)/conf/hk_conf.c : $(APPLICATIONS_CONF_DIR)/hk_conf.csv
	@echo "  PY  $(@F), $(@F:.c=.h)"
	@${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/hk-parser.py -i $^ -o $(@D)

$(PRE_BUILD_DIR)/conf/peripherals_conf.c : $(APPLICATIONS_CONF_DIR)/peripherals_conf.csv
	@echo "  PY  $(@F), $(@F:.c=.h)"
	@${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/peripherals-parser.py -i $^ -o $(@D)

$(PRE_BUILD_DIR)/conf/timers_conf.c : $(APPLICATIONS_CONF_DIR)/timers_conf.csv
	@echo "  PY  $(@F), $(@F:.c=.h)"
	@${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/timers-parser.py -i $^ -o $(@D)

# Linker script recipe
linker-script : $(LD_SCRIPT)

LD_INC = -I$(APPLICATIONS_DIR) -I$(KERNEL_DIR) -I$(PUS_DIR) -I$(IRIDIUMDRV_DIR) -I$(BSP_DIR)

$(LD_SCRIPT) : $(RAW_LD_SCRIPT)
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) -w $(LD_INC) -E -P -x c $^ -o $@

# Pre-build footer
pre-build-end :
	@echo "Build done"
	@echo ""

# Pre-build clean recipes
pre-build-clean :
	@echo "Cleaning PRE-BUILD build directory"
	@rm -rf $(PRE_BUILD_DIR)
	@echo "Done"

endif # BUILD_PRE_BUILD_MK #