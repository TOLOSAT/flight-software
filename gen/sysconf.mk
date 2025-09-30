# Pre Building Makefile

ifndef SYSCONF_MK
SYSCONF_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk

##############################################
############# SYSCONF DIRECTORIES ############
##############################################

SYSCONF_DIR	= $(BUILD_DIR)/applications/conf

##############################################
################### SYSCONF #################
##############################################

# JSON configuration file
CONF_JSON = $(APPLICATIONS_DIR)/system.json

# List of generated configuration .c files
SYS_CONF_SRCS = $(SYSCONF_DIR)/tasks_conf.c \
				$(SYSCONF_DIR)/buffers_conf.c \
				$(SYSCONF_DIR)/mutex_conf.c \
				$(SYSCONF_DIR)/fs_conf.c \
				$(SYSCONF_DIR)/timers_conf.c

# System configuration recipes
.PHONY : sysconf sysconf-start conf-files sysconf-end sysconf-clean
sysconf : sysconf-start conf-files sysconf-end

# System configuration header
sysconf-start :
	@echo "============================="
	@echo "===   SYSCONF GENERATION  ==="
	@echo "============================="
	@echo "Files to sysconf: $(words $(SYS_CONF_SRCS))"
	@echo "Start sysconfing:"

# System configuration files generation
conf-files : $(SYSCONF_DIR)/system-conf.stamp

$(SYSCONF_DIR)/system-conf.stamp : $(CONF_JSON)
	@mkdir -p $(@D)
	@touch $@
	@echo "  PY  tasks_conf.c, tasks_conf.h"; echo "tasks_conf.c, tasks_conf.h" >> $@
	@echo "  PY  buffers_conf.c, buffers_conf.h"; echo "buffers_conf.c, buffers_conf.h" >> $@
	@echo "  PY  mutex_conf.c, mutex_conf.h"; echo "mutex_conf.c, mutex_conf.h" >> $@
	@echo "  PY  fs_conf.c, fs_conf.h"; echo "fs_conf.c, fs_conf.h" >> $@
	@echo "  PY  timers_conf.c, timers_conf.h"; echo "timers_conf.c, timers_conf.h" >> $@
	@${PYTHON} $(TOOLS_DIR)/system-parser.py -i $(CONF_JSON) -o $(SYSCONF_DIR)

# System configuration footer
sysconf-end :
	@echo "Build done"
	@echo ""

# System configuration clean recipes
sysconf-clean :
	@echo "Cleaning APP-CONF build directory"
	@rm -rf $(SYSCONF_DIR)
	@echo "Done"

endif # SYSCONF_MK #