# Pre Building Makefile

ifndef APP_CONF_MK
APP_CONF_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk

##############################################
############# APP_CONF DIRECTORIES ###########
##############################################

APP_CONF_SCRIPTS_DIR	= $(TOOLS_DIR)
APP_CONF_DIR			= $(BUILD_DIR)/applications/conf

##############################################
################### APP_CONF #################
##############################################

# JSON configuration file
CONF_JSON = $(APPLICATIONS_DIR)/system.json

# List of generated configuration .c files
SYS_CONF_SRCS = $(APP_CONF_DIR)/tasks_conf.c \
				$(APP_CONF_DIR)/buffers_conf.c \
				$(APP_CONF_DIR)/mutex_conf.c \
				$(APP_CONF_DIR)/fs_conf.c \
				$(APP_CONF_DIR)/hk_conf.c \
				$(APP_CONF_DIR)/timers_conf.c

SYS_CONF_OBJS = $(SYS_CONF_SRCS:.c=.o)

# Application configuration recipes
.PHONY : app-conf app-conf-start conf-files app-conf-end app-conf-clean
app-conf : app-conf-start conf-files app-conf-end

# Application configuration header
app-conf-start :
	@echo "============================="
	@echo "===  APPLICATION CONF GEN ==="
	@echo "============================="
	@echo "Files to app-conf: $(words $(SYS_CONF_SRCS))"
	@echo "Start app-confing:"

# Application configuration files generation
conf-files : $(SYS_CONF_OBJS)

$(SYS_CONF_SRCS) : $(APP_CONF_DIR)/system-conf.stamp

$(APP_CONF_DIR)/system-conf.stamp : $(CONF_JSON)
	@mkdir -p $(@D)
	@touch $@
	@echo "  PY  tasks_conf.c, tasks_conf.h"; echo "tasks_conf.c, tasks_conf.h" >> $@
	@echo "  PY  buffers_conf.c, buffers_conf.h"; echo "buffers_conf.c, buffers_conf.h" >> $@
	@echo "  PY  mutex_conf.c, mutex_conf.h"; echo "mutex_conf.c, mutex_conf.h" >> $@
	@echo "  PY  fs_conf.c, fs_conf.h"; echo "fs_conf.c, fs_conf.h" >> $@
	@echo "  PY  timers_conf.c, timers_conf.h"; echo "timers_conf.c, timers_conf.h" >> $@
	@echo "  PY  hk_conf.c, hk_conf.h"; echo "hk_conf.c, hk_conf.h" >> $@
	@${PYTHON} $(APP_CONF_SCRIPTS_DIR)/system-parser.py -i $(CONF_JSON) -o $(APP_CONF_DIR)

$(APP_CONF_DIR)/%.o : $(APP_CONF_DIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)

# Application configuration footer
app-conf-end :
	@echo "Build done"
	@echo ""

# Application configuration clean recipes
app-conf-clean :
	@echo "Cleaning APP-CONF build directory"
	@rm -rf $(APP_CONF_DIR)
	@echo "Done"

endif # APP_CONF_MK #