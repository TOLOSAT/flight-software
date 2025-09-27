# Pre Building Makefile

ifndef BUILD_PRE_BUILD_MK
BUILD_PRE_BUILD_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk

##############################################
############ PRE-BUILD DIRECTORIES ###########
##############################################

PRE_BUILD_SCRIPTS_DIR	= $(TOOLS_DIR)
PRE_BUILD_DIR			= $(BUILD_DIR)/pre-build

##############################################
################## PRE-BUILD #################
##############################################

# JSON configuration file
CONF_JSON = $(APPLICATIONS_DIR)/system.json
BSP_JSON = $(KERNEL_DIR)/bsp/$(BOARD)-BSP/bsp.json

# List of generated configuration .c files
SYS_CONF_SRCS = $(PRE_BUILD_DIR)/conf/tasks_conf.c \
				$(PRE_BUILD_DIR)/conf/buffers_conf.c \
				$(PRE_BUILD_DIR)/conf/mutex_conf.c \
				$(PRE_BUILD_DIR)/conf/fs_conf.c \
				$(PRE_BUILD_DIR)/conf/hk_conf.c \
				$(PRE_BUILD_DIR)/conf/timers_conf.c
BSP_CONF_SRCS = $(PRE_BUILD_DIR)/conf/peripherals_conf.c \
				$(PRE_BUILD_DIR)/conf/system_peripherals_conf.c \
				$(PRE_BUILD_DIR)/conf/memories_conf.c

# Autoconf file
AUTOCONF_SRC = $(PRE_BUILD_DIR)/autoconf.h

# Pre-build recipes
.PHONY : pre-build pre-build-start autoconf conf-files linker-script pre-build-end pre-build-clean
pre-build : pre-build-start autoconf conf-files linker-script pre-build-end

# Pre-build header
pre-build-start :
	@echo "============================="
	@echo "===       PRE BUILD       ==="
	@echo "============================="
	@echo "Files to pre-build: $(words $(SYS_CONF_SRCS) $(BSP_CONF_SRCS) $(AUTOCONF_SRC))"
	@echo "Start pre-building:"

# Autoconf recipes
autoconf : $(AUTOCONF_SRC)

$(AUTOCONF_SRC) : $(CONFIG_FILE)
	@echo "  PY  $(@F)"
	@${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/config-parser.py -i $^ -o $(@D)

# Configuration files generation
conf-files : $(PRE_BUILD_DIR)/conf/system-conf.stamp $(PRE_BUILD_DIR)/conf/bsp-conf.stamp
$(SYS_CONF_SRCS): $(PRE_BUILD_DIR)/conf/system-conf.stamp
$(BSP_CONF_SRCS): $(PRE_BUILD_DIR)/conf/bsp-conf.stamp

$(PRE_BUILD_DIR)/conf/system-conf.stamp : $(CONF_JSON)
	@mkdir -p $(@D)
	@touch $@
	@echo "  PY  tasks_conf.c, tasks_conf.h"; echo "tasks_conf.c, tasks_conf.h" >> $@
	@echo "  PY  buffers_conf.c, buffers_conf.h"; echo "buffers_conf.c, buffers_conf.h" >> $@
	@echo "  PY  mutex_conf.c, mutex_conf.h"; echo "mutex_conf.c, mutex_conf.h" >> $@
	@echo "  PY  fs_conf.c, fs_conf.h"; echo "fs_conf.c, fs_conf.h" >> $@
	@echo "  PY  timers_conf.c, timers_conf.h"; echo "timers_conf.c, timers_conf.h" >> $@
	@echo "  PY  hk_conf.c, hk_conf.h"; echo "hk_conf.c, hk_conf.h" >> $@
	@${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/system-parser.py -i $(CONF_JSON) -o $(PRE_BUILD_DIR)/conf

$(PRE_BUILD_DIR)/conf/bsp-conf.stamp : $(BSP_JSON)
	@mkdir -p $(@D)
	@echo "  PY  peripherals_conf.c, peripherals_conf.h"; echo "peripherals_conf.c, peripherals_conf.h" >> $@
	@echo "  PY  system_peripherals_conf.c, system_peripherals_conf.h"; echo "system_peripherals_conf.c, system_peripherals_conf.h" >> $@
	@echo "  PY  memories_conf.c, memories_conf.h"; echo "memories_conf.c, memories_conf.h" >> $@
	@${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/bsp-parser.py -i $(BSP_JSON) -o $(PRE_BUILD_DIR)/conf

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