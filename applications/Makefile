# Applications Building Makefile

ifndef BUILD_APPLICATIONS_MK
BUILD_APPLICATIONS_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/paths.mk
include gen/cc-settings.mk

##############################################
################ CONFIGURATION ###############
##############################################

APPLICATIONS = aocs gravimetry iridium power thermal tmtc
APPLICATION_DEPENDANCIES = iridium pus

##############################################
########## APPLICATIONS DIRECTORIES ##########
##############################################

# Applications Directories
APPLICATIONS_CONF_DIR	= $(APPLICATIONS_DIR)/conf
APPLICATIONS_OBJDIR		= $(BUILD_DIR)/applications

# Dependancies
APPLICATION_DEPENDANCIES_INCDIRS = $(addprefix $(MIDDLEWARES_DIR)/, $(addsuffix -library/inc,$(APPLICATION_DEPENDANCIES)))
APPLICATION_DEPENDANCIES_LIBS    = $(foreach lib,$(APPLICATION_DEPENDANCIES),-l$(lib))

##############################################
################ APPLICATIONS ################
##############################################

# Applications files
APPLICATIONS_SRCS = $(foreach app,$(APPLICATIONS),$(wildcard $(APPLICATIONS_DIR)/$(app)/src/*.c)) $(SYS_CONF_SRCS)
APPLICATIONS_INCS = $(foreach app,$(APPLICATIONS),$(APPLICATIONS_DIR)/$(app)/inc)
APPLICATIONS_OBJS = $(patsubst $(APPLICATIONS_DIR)/%.c,$(APPLICATIONS_OBJDIR)/%.o, \
					$(patsubst $(PRE_BUILD_DIR)/%.c,$(APPLICATIONS_OBJDIR)/conf/%.o, \
					$(APPLICATIONS_SRCS)))
APPLICATIONS_LIB  = $(LIBS_DIR)/libapplications.a

# Applications flags
APPLICATIONS_CFLAGS   = $(PROJECT_CFLAGS)
APPLICATIONS_INCDIRS  = $(APPLICATIONS_INCS) $(APPLICATION_DEPENDANCIES_INCDIRS) \
						$(KERNEL_HEADERS) $(PRE_BUILD_DIR) \
						$(BUILD_DIR)/kernel/conf # TO DO REMOVE
APPLICATIONS_INCFLAGS = $(addprefix -I,$(APPLICATIONS_INCDIRS))

# Include dependencies
-include $(APPLICATIONS_OBJS:.o=.d)

# Applications recipes
.PHONY : applications applications-start applications-end applications-clean
applications : pre-build applications-start $(APPLICATIONS_LIB) applications-end

# Build header
applications-start :
	@echo "============================="
	@echo "===      APPLICATIONS     ==="
	@echo "============================="
	@echo "Files to compile: $(words $(APPLICATIONS_SRCS))"
	@echo "Compilation Flags:"
	@echo $(APPLICATIONS_CFLAGS)
	@echo "Include Paths:"
	@$(foreach dir,$(patsubst $(WORKSPACE)/%,%,$(APPLICATIONS_INCDIRS)),echo "  - $(dir)";)
	@echo "Start building:"

# Building recipes
$(APPLICATIONS_OBJDIR)/%.o : $(APPLICATIONS_DIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(APPLICATIONS_CFLAGS) $(APPLICATIONS_INCFLAGS) $< -o $@

$(APPLICATIONS_OBJDIR)/conf/%.o : $(PRE_BUILD_DIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(APPLICATIONS_CFLAGS) $(APPLICATIONS_INCFLAGS) $< -o $@

# Library generation
$(APPLICATIONS_LIB) : $(APPLICATIONS_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
applications-end :
	@echo "Build done"
	@echo ""

# Clean recipe
applications-clean :
	@echo "Cleaning APPLICATIONS build directory ..."
	@rm -rf $(APPLICATIONS_OBJDIR)
	@rm -rf $(APPLICATIONS_LIB)
	@echo "Done"

endif # BUILD_APPLICATIONS_MK #