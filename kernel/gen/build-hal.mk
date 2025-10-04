# Makefile including all build recipes for kernel third parties

ifndef BUILD_HAL_MK
BUILD_HAL_MK := yes

##############################################
############# DIRECTORIES & FILES ############
##############################################

# Directories
HALs_DIR		= $(THIRD_PARTIES_DIR)/HALs
HAL_DIR			= $(HALs_DIR)/HAL-$(CHIP_FAMILLY)
HAL_INCDIR		= $(HAL_DIR)/Inc
HAL_SRCDIR		= $(HAL_DIR)/Src
HAL_OBJDIR		= $(BUILD_DIR)/third-parties/hal

# Files
include $(THIRD_PARTIES_CONFDIR)/HAL_SRCS_$(CHIP_FAMILLY).mk
HAL_OBJS  = $(subst $(HAL_SRCDIR)/,$(HAL_OBJDIR)/,$(HAL_SRCS:.c=.o))
HAL_LIB   = $(LIBS_DIR)/libhal.a

##############################################
#################### FLAGS ###################
##############################################

HAL_CFLAGS		=	$(CFLAGS) \
					-D$(CHIP) -D$(CHIP_FAMILLY) $(CORE_SELECT) \
					-Wno-unused-variable -Wno-unused-parameter
HAL_INCDIRS		=	$(HAL_INCDIR) $(HAL_INCDIR)/Legacy $(THIRD_PARTIES_CONFDIR) \
					$(CMSIS_INCDIR) $(CMSIS_INCDIR_DEVICE) \
					$(PRE_BUILD_DIR)
HAL_INCFLAGS	=	$(addprefix -I,$(HAL_INCDIRS))

##############################################
################ BUILD RECIPES ###############
##############################################

.PHONY : hal hal-start hal-end hal-clean
hal : hal-start $(HAL_LIB) hal-end

# Include dependencies
-include $(HAL_OBJS:.o=.d)

# Build header
hal-start :
	@echo "============================="
	@echo "===          HAL          ==="
	@echo "============================="
	@echo "Files to compile: $(words $(HAL_SRCS))"
	@echo "Compilation Flags:"
	@echo $(HAL_CFLAGS)
	@echo "Include Paths:"
	@$(foreach dir,$(patsubst $(WORKSPACE)/%,%,$(HAL_INCDIRS)),echo "  - $(dir)";)
	@echo "Start building:"

# Building recipes
$(HAL_OBJDIR)/%.o : $(HAL_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(HAL_CFLAGS) $(HAL_INCFLAGS) $< -o $@

# Library generation
$(HAL_LIB) : $(HAL_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
hal-end :
	@echo "Build done"
	@echo ""

# Clean recipe
hal-clean :
	@echo "Cleaning HAL build directory ..."
	@rm -rf $(HAL_OBJDIR)
	@rm -rf $(HAL_LIB)
	@echo "Done"

endif # BUILD_HAL_MK #