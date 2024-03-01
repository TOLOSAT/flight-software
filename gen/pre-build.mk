# Pre Building Makefile

##############################################
################## PRE-BUILD #################
##############################################

pre-build :
	${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/tasks-parser.py -i $(APPLICATION_CONF_DIR)/tasks_conf.csv -o $(PRE_BUILD_DIR)/conf
	${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/buffers-parser.py -i $(APPLICATION_CONF_DIR)/buffers_conf.csv -o $(PRE_BUILD_DIR)/conf
	${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/mutex-parser.py -i $(APPLICATION_CONF_DIR)/mutex_conf.csv -o $(PRE_BUILD_DIR)/conf
	${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/fs-parser.py -i $(APPLICATION_CONF_DIR)/fs_conf.csv -o $(PRE_BUILD_DIR)/conf
	${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/hk-parser.py -i $(APPLICATION_CONF_DIR)/hk_conf.csv -o $(PRE_BUILD_DIR)/conf
	@echo "****************************"
	@echo "****   Pre Build Done   ****"
	@echo "****************************"