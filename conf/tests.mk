##############################################
#################### TESTS ###################
##############################################

# Verify that ./tests/test-xx path exists
ifeq ($(findstring test-,$(MAKECMDGOALS)),test-)
	ifeq ($(wildcard $(APPLICATION_DIR)),)
        print := $(error Directory $(APPLICATION_DIR) does not exist.)
    endif
endif

test-% : build
	@echo $(APPLICATION_DIR)