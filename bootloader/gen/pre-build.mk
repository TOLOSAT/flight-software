# Pre Building Makefile

##############################################
################## PRE-BUILD #################
##############################################

# Pre-build recipes
pre-build : pre-build-start linker-script pre-build-end

# Pre-build header
pre-build-start :
	@echo "============================="
	@echo "===       PRE BUILD       ==="
	@echo "============================="
	@echo "Files to pre-build: $(words $(BSP_LD_SCRIPT))"
	@echo "Start pre-building:"

# Linker script recipe
linker-script : $(LD_SCRIPT)

$(LD_SCRIPT) : $(BSP_LD_SCRIPT)
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@cp $^ $@

# Pre-build footer
pre-build-end :
	@echo "Build done"
	@echo ""