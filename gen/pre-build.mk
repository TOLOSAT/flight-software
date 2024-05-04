# Pre Building Makefile

##############################################
################## PRE-BUILD #################
##############################################

# CSV conf files

APPLICATION_CONF_CSV = $(wildcard $(APPLICATION_CONF_DIR)/*.csv)
APPLICATION_CONF_SRCS = $(subst $(APPLICATION_CONF_DIR)/,$(PRE_BUILD_DIR)/conf/,$(APPLICATION_CONF_CSV:.csv=.c))

$(PRE_BUILD_DIR)/conf/tasks_conf.c : $(APPLICATION_CONF_DIR)/tasks_conf.csv
	${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/tasks-parser.py -i $^ -o $(@D)

$(PRE_BUILD_DIR)/conf/buffers_conf.c : $(APPLICATION_CONF_DIR)/buffers_conf.csv
	${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/buffers-parser.py -i $^ -o $(@D)

$(PRE_BUILD_DIR)/conf/mutex_conf.c : $(APPLICATION_CONF_DIR)/mutex_conf.csv
	${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/mutex-parser.py -i $^ -o $(@D)

$(PRE_BUILD_DIR)/conf/fs_conf.c : $(APPLICATION_CONF_DIR)/fs_conf.csv
	${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/fs-parser.py -i $^ -o $(@D)

$(PRE_BUILD_DIR)/conf/hk_conf.c : $(APPLICATION_CONF_DIR)/hk_conf.csv
	${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/hk-parser.py -i $^ -o $(@D)

# LD script generation

LD_INC = -I$(CORE_DIR) -I$(GENERIC_HAL_DIR) -I$(LIBPUS_DIR)

$(LD_SCRIPT) : $(BSP_LD_SCRIPT)
	mkdir -p $(@D)
	cp $^ $@
	$(CC) -w $(LD_INC) -E -P -x c $^ > $@

# Pre-build recipe
pre-build : $(APPLICATION_CONF_SRCS) $(LD_SCRIPT)
	@echo "****************************"
	@echo "****   Pre Build Done   ****"
	@echo "****************************"