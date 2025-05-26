# Pre Building Makefile

ifndef BUILD_PRE_BUILD_MK
BUILD_PRE_BUILD_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk

##############################################
################## PRE-BUILD #################
##############################################

# Autoconf file
AUTOCONF_SRC = $(PRE_BUILD_DIR)/autoconf.h

# Pre-build recipes
.PHONY : pre-build pre-build-start autoconf linker-script pre-build-end pre-build-clean
pre-build : pre-build-start autoconf linker-script pre-build-end

# Pre-build header
pre-build-start :
	@echo "============================="
	@echo "===       PRE BUILD       ==="
	@echo "============================="
	@echo "Files to pre-build: $(words $(RAW_LD_SCRIPT) $(AUTOCONF_SRC))"
	@echo "Start pre-building:"

# Autoconf recipes
autoconf : $(AUTOCONF_SRC)

$(AUTOCONF_SRC) : $(CONFIG_FILE)
	@echo "  PY  $(@F)"
	@${PYTHON} $(PRE_BUILD_SCRIPTS_DIR)/config-parser.py -i $^ -o $(@D)

# Linker script recipe
linker-script : $(LD_SCRIPT)

$(LD_SCRIPT) : $(RAW_LD_SCRIPT)
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@cp $^ $@

# Pre-build footer
pre-build-end :
	@echo "Build done"
	@echo ""

# Pre-build clean recipes
pre-build-clean :
	@echo "Cleaning PRE-BUILD build directory"
	@rm -rf $(PRE_BUILD_DIR)
	@echo "Done"

endif # BUILD_PRE_BUILD_MK #