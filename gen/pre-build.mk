# Pre Building Makefile

##############################################
################## PRE-BUILD #################
##############################################

$(LD_SCRIPT) : $(BSP_LD_SCRIPT)
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@cp $^ $@

pre-build-start :
	@echo "**************************************"
	@echo "******   Pre-Build Start Build   *****"
	@echo "**************************************"

pre-build-end :
	@echo "**************************************"
	@echo "******   Pre Build Build Done   ******"
	@echo "**************************************"
	@echo

pre-build : pre-build-start $(LD_SCRIPT) pre-build-end