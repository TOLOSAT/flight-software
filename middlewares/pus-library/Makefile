# MIDDLEWARES Building Makefile

ifndef BUILD_PUS_MK
BUILD_PUS_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk
include gen/cc_settings.mk


##############################################
############### PUS DIRECTORIES ##############
##############################################

MIDDLEWARES_OBJDIR	= $(BUILD_DIR)/middlewares

# PUS LIBRARY Directories
PUS_DIR		= $(MIDDLEWARES_DIR)/pus-library
PUS_INCDIR	= $(PUS_DIR)/inc
PUS_SRCDIR	= $(PUS_DIR)/src
PUS_OBJDIR	= $(MIDDLEWARES_OBJDIR)/pus

##############################################
################# PUS LIBRARY ################
##############################################

# PUS library files
PUS_SRCS = $(wildcard $(PUS_SRCDIR)/*.c $(PUS_SRCDIR)/*/*.c)
PUS_OBJS = $(subst $(PUS_SRCDIR)/,$(PUS_OBJDIR)/,$(PUS_SRCS:.c=-$(BUILD_TYPE).o))
PUS_LIB  = $(LIBS_DIR)/libpus-$(BUILD_TYPE).a

# PUS LIBRARY flags
PUS_CFLAGS    = $(PROJECT_CFLAGS)
PUS_INCFLAGS  = -I$(PUS_INCDIR)
PUS_INCFLAGS += -I$(KERNEL_INCLUDES) -I$(PRE_BUILD_DIR)

# Include dependencies
-include $(PUS_OBJS:.o=.d)

# PUS library recipes
.PHONY : pus pus-start pus-end pus-clean
pus : pus-start $(PUS_LIB) pus-end

# Build header
pus-start :
	@echo "============================="
	@echo "===          PUS          ==="
	@echo "============================="
	@echo "Files to compile: $(words $(PUS_SRCS))"
	@echo "Compilation Flags:"
	@echo $(PUS_CFLAGS)
	@echo "Include Paths:"
	@echo $(PUS_INCFLAGS)
	@echo "Version Flags:"
	@echo $(VERSION_FLAGS)
	@echo "Start building:"

# Building recipes
$(PUS_OBJDIR)/%-$(BUILD_TYPE).o : $(PUS_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(PUS_CFLAGS) $(PUS_INCFLAGS) $(VERSION_FLAGS) $< -o $@

# Library generation
$(PUS_LIB) : $(PUS_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
pus-end :
	@echo "Build done"
	@echo ""

# Clean recipe
pus-clean :
	@echo "Cleaning PUS build directory ..."
	@rm -rf $(PUS_OBJDIR)
	@rm -rf $(PUS_LIB)
	@echo "Done"

endif # BUILD_PUS_MK #