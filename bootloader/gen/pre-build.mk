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
	@$(eval start_time=$(shell date +%s))

# Linker script recipe
linker-script : $(LD_SCRIPT)

$(LD_SCRIPT) : $(BSP_LD_SCRIPT)
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@cp $^ $@

# Pre-build footer
pre-build-end :
	@$(eval end_time=$(shell date +%s))
	@echo "Build done ($$(($(end_time)-$(start_time))) seconds elapsed)"
	@echo ""