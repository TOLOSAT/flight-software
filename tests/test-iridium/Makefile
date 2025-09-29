# Applications Building Makefile

ifndef BUILD_APPLICATIONS_MK
BUILD_APPLICATIONS_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk
include gen/cc-settings.mk

##############################################
################ CONFIGURATION ###############
##############################################

APPLICATIONS = .
APPLICATION_DEPENDANCIES = iridium pus

##############################################
########## APPLICATIONS DIRECTORIES ##########
##############################################

# Applications Directories
APPLICATIONS_CONF_DIR	= $(APPLICATIONS_DIR)/conf
APPLICATIONS_OBJDIR		= $(BUILD_DIR)/applications

# Dependancies
APPLICATION_DEPENDANCIES_LIBS = $(foreach lib,$(APPLICATION_DEPENDANCIES),-l$(lib))

##############################################
################ APPLICATIONS ################
##############################################

# Applications files
APPLICATIONS_SRCS = $(foreach app,$(APPLICATIONS),$(wildcard $(APPLICATIONS_DIR)/$(app)/src/*.c))
APPLICATIONS_INCS = $(foreach app,$(APPLICATIONS),-I$(APPLICATIONS_DIR)/$(app)/inc)
APPLICATIONS_OBJS = $(subst $(APPLICATIONS_DIR)/,$(APPLICATIONS_OBJDIR)/,$(APPLICATIONS_SRCS:.c=.o))
APPLICATIONS_LIB  = $(LIBS_DIR)/libapplications.a

# Applications flags
APPLICATIONS_CFLAGS    = $(PROJECT_CFLAGS)
APPLICATIONS_INCFLAGS  = $(APPLICATIONS_INCS) \
						 -I$(KERNEL_HEADERS) -I$(APP_CONF_DIR) \
						 -I$(PUS_INCDIR) \
						 -I$(IRIDIUM_INCDIR)

# Include dependencies
-include $(APPLICATIONS_OBJS:.o=.d)

# Applications recipes
.PHONY : applications applications-start applications-end applications-clean
applications : applications-start $(APPLICATIONS_LIB) applications-end

# Build header
applications-start :
	@echo "============================="
	@echo "===      APPLICATIONS     ==="
	@echo "============================="
	@echo "Files to compile: $(words $(APPLICATIONS_SRCS))"
	@echo "Compilation Flags:"
	@echo $(APPLICATIONS_CFLAGS)
	@echo "Include Paths:"
	@echo $(APPLICATIONS_INCFLAGS)
	@echo "Start building:"

# Building recipes
$(APPLICATIONS_OBJDIR)/%.o : $(APPLICATIONS_DIR)/%.c
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