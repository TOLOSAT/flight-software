# Makefile including autoconf build recipes

ifndef KERNEL_AUTOCONF_MK
KERNEL_AUTOCONF_MK := yes

##############################################
############# DIRECTORIES & FILES ############
#############################################

# Directories
KERNEL_AUTOCONF_DIR	= $(BUILD_DIR)/kernel/conf

# Files
BSP_JSON 		= ./bsp/$(BOARD)-BSP/bsp.json
AUTOCONF_SRC 	= $(KERNEL_AUTOCONF_DIR)/autoconf.h
BSP_CONF_SRCS	= $(KERNEL_AUTOCONF_DIR)/peripherals_conf.c \
				  $(KERNEL_AUTOCONF_DIR)/system_peripherals_conf.c \
				  $(KERNEL_AUTOCONF_DIR)/memories_conf.c

##############################################
################ BUILD RECIPES ###############
##############################################

.PHONY : pre-build pre-build-start autoconf conf-files pre-build-end
pre-build : pre-build-start autoconf conf-files pre-build-end

# Pre-build header
pre-build-start :
	@echo "============================="
	@echo "===    KERNEL PRE BUILD   ==="
	@echo "============================="
	@echo "Files to pre-build: $(words $(AUTOCONF_SRC) $(BSP_CONF_SRCS))"
	@echo "Start pre-building:"

# Autoconf recipes
autoconf : $(AUTOCONF_SRC)

$(AUTOCONF_SRC) : $(CONFIG_FILE)
	@echo "  PY  $(@F)"
	@mkdir -p $(@D)
	@${PYTHON} tools/config-parser.py -i $^ -o $(@D)

# Configuration files generation
conf-files : $(KERNEL_AUTOCONF_DIR)/bsp-conf.stamp
$(BSP_CONF_SRCS): $(KERNEL_AUTOCONF_DIR)/bsp-conf.stamp

$(KERNEL_AUTOCONF_DIR)/bsp-conf.stamp : $(BSP_JSON)
	@mkdir -p $(@D)
	@echo "  PY  peripherals_conf.c, peripherals_conf.h"; echo "peripherals_conf.c, peripherals_conf.h" >> $@
	@echo "  PY  system_peripherals_conf.c, system_peripherals_conf.h"; echo "system_peripherals_conf.c, system_peripherals_conf.h" >> $@
	@echo "  PY  memories_conf.c, memories_conf.h"; echo "memories_conf.c, memories_conf.h" >> $@
	@${PYTHON} tools/bsp-parser.py -i $(BSP_JSON) -o $(KERNEL_AUTOCONF_DIR)

# Pre-build footer
pre-build-end :
	@echo "Build done"
	@echo ""

endif # KERNEL_AUTOCONF_MK #