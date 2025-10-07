# Makefile with library paths

ifndef PATH_MK
PATH_MK := yes

##############################################
############# LIBRARY DIRECTORIES ############
##############################################

# Depending if the workspace is provided by the top level or not
ifeq ($(origin WORKSPACE), undefined)
LIB_DIR 	= $(CURDIR)
BUILD_DIR 	= $(LIB_DIR)/build
else
LIB_DIR  	= $(WORKSPACE)/middlewares/$(LIB_NAME)-library
BUILD_DIR   = $(WORKSPACE)/build
endif

GEN_DIR          	= $(LIB_DIR)/gen
OUTPUT_DIR         	= $(BUILD_DIR)/libs

endif # PATH_MK #