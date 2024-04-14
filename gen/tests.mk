# Test Building Makefile

##############################################
#################### TESTS ###################
##############################################

# Verify that ./tests/test-xx path exists
ifeq ($(findstring test-,$(MAKECMDGOALS)),test-)
ifeq ($(wildcard $(APPLICATION_DIR)),)
$(error Directory $(APPLICATION_DIR) does not exist.)
endif
endif

test-% : clean pre-build build upload
	@echo $(APPLICATION_DIR)