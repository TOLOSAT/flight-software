# Pre Building Makefile

##############################################
############### Pre Build Files ##############
##############################################

pre-build :
	${PYTHON} $(TOOLS_DIR)/pre-build/tasks-parser.py -i $(APPLICATION_CONF_DIR)/tasks_conf.csv -o $(PRE_BUILD_DIR)/conf
	${PYTHON} $(TOOLS_DIR)/pre-build/buffers-parser.py -i $(APPLICATION_CONF_DIR)/buffers_conf.csv -o $(PRE_BUILD_DIR)/conf
	${PYTHON} $(TOOLS_DIR)/pre-build/mutex-parser.py -i $(APPLICATION_CONF_DIR)/mutex_conf.csv -o $(PRE_BUILD_DIR)/conf
	${PYTHON} $(TOOLS_DIR)/pre-build/fs-parser.py -i $(APPLICATION_CONF_DIR)/fs_conf.csv -o $(PRE_BUILD_DIR)/conf
	${PYTHON} $(TOOLS_DIR)/pre-build/hk-parser.py -i $(APPLICATION_CONF_DIR)/hk_conf.csv -o $(PRE_BUILD_DIR)/conf
	@echo "****************************"
	@echo "****   Pre Build Done   ****"
	@echo "****************************"