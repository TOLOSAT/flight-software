# Pre Building Makefile

##############################################
############### Pre Build Files ##############
##############################################

pre-build :
	${PYTHON} $(TOOLS_DIR)/task-parser.py -i $(APPLICATION_CONF_DIR)/task_conf.csv -o $(PRE_BUILD_DIR)/conf