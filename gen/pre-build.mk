# Pre Building Makefile

##############################################
############### Pre Build Files ##############
##############################################

pre-build :
	${PYTHON} $(TOOLS_DIR)/tasks-parser.py -i $(APPLICATION_CONF_DIR)/tasks_conf.csv -o $(PRE_BUILD_DIR)/conf
	${PYTHON} $(TOOLS_DIR)/buffers-parser.py -i $(APPLICATION_CONF_DIR)/buffers_conf.csv -o $(PRE_BUILD_DIR)/conf
	@echo "****************************"
	@echo "****   Pre Build Done   ****"
	@echo "****************************"