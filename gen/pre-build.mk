# Pre Building Makefile

ifndef PRE_BUILD_MK
PRE_BUILD_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk

##############################################
################## PRE-BUILD #################
##############################################

# JSON configuration file
CONF_JSON = $(APPLICATIONS_DIR)/system.json

# List of generated configuration .c files
SYS_CONF_SRCS = $(PRE_BUILD_DIR)/tasks_conf.c \
				$(PRE_BUILD_DIR)/buffers_conf.c \
				$(PRE_BUILD_DIR)/mutex_conf.c \
				$(PRE_BUILD_DIR)/fs_conf.c \
				$(PRE_BUILD_DIR)/timers_conf.c

# System configuration recipes
.PHONY : pre-build pre-build-start conf-files pre-build-end pre-build-clean
pre-build : pre-build-start conf-files pre-build-end

# System configuration header
pre-build-start :
	@echo "============================="
	@echo "===       PRE BUILD       ==="
	@echo "============================="
	@echo "Files to pre-build: $(words $(SYS_CONF_SRCS))"
	@echo "Start pre-building:"

# System configuration files generation
conf-files : $(SYS_CONF_SRCS)

$(SYS_CONF_SRCS) : $(PRE_BUILD_DIR)/system-conf.stamp

$(PRE_BUILD_DIR)/system-conf.stamp : $(CONF_JSON)
	@mkdir -p $(@D)
	@touch $@
	@echo "  PY  tasks_conf.c, tasks_conf.h"; echo "tasks_conf.c, tasks_conf.h" >> $@
	@echo "  PY  buffers_conf.c, buffers_conf.h"; echo "buffers_conf.c, buffers_conf.h" >> $@
	@echo "  PY  mutex_conf.c, mutex_conf.h"; echo "mutex_conf.c, mutex_conf.h" >> $@
	@echo "  PY  fs_conf.c, fs_conf.h"; echo "fs_conf.c, fs_conf.h" >> $@
	@echo "  PY  timers_conf.c, timers_conf.h"; echo "timers_conf.c, timers_conf.h" >> $@
	@${PYTHON} $(TOOLS_DIR)/system-parser.py -i $(CONF_JSON) -o $(PRE_BUILD_DIR)

# System configuration footer
pre-build-end :
	@echo "Build done"
	@echo ""

# System configuration clean recipes
pre-build-clean :
	@echo "Cleaning APP-CONF build directory"
	@rm -rf $(PRE_BUILD_DIR)
	@echo "Done"

endif # PRE_BUILD_MK #