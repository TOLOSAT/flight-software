# MIDDLEWARES Building Makefile

ifndef BUILD_IRIDIUM_MK
BUILD_IRIDIUM_MK := yes

##############################################
################## INCLUDES ##################
##############################################

include gen/settings.mk
include gen/path.mk
include gen/cc_settings.mk


##############################################
############# IRIDIUM DIRECTORIES ############
##############################################

MIDDLEWARES_OBJDIR	= $(BUILD_DIR)/middlewares

# IRIDIUM_DRV Directories
IRIDIUM_DIR		= $(MIDDLEWARES_DIR)/iridium-library
IRIDIUM_INCDIR	= $(IRIDIUM_DIR)/inc
IRIDIUM_SRCDIR	= $(IRIDIUM_DIR)/src
IRIDIUM_OBJDIR	= $(MIDDLEWARES_OBJDIR)/iridium

##############################################
############### IRIDIUM DRIVER ###############
##############################################

# Iridium driver files
IRIDIUM_SRCS = $(wildcard $(IRIDIUM_SRCDIR)/*.c $(IRIDIUM_SRCDIR)/*/*.c)
IRIDIUM_OBJS = $(subst $(IRIDIUM_SRCDIR)/,$(IRIDIUM_OBJDIR)/,$(IRIDIUM_SRCS:.c=-$(BUILD_TYPE).o))
IRIDIUM_LIB  = $(LIBS_DIR)/libiridium-$(BUILD_TYPE).a

# Iridium driver flags
IRIDIUM_CFLAGS    = $(PROJECT_CFLAGS)
IRIDIUM_INCFLAGS  = -I$(IRIDIUM_INCDIR)
IRIDIUM_INCFLAGS += -I$(PUS_INCDIR)
IRIDIUM_INCFLAGS += -I$(KERNEL_INCLUDES) -I$(PRE_BUILD_DIR)

# Include dependencies
-include $(IRIDIUM_OBJS:.o=.d)

# Iridium Driver recipes
.PHONY : iridium iridium-start iridium-end iridium-clean
iridium : iridium-start $(IRIDIUM_LIB) iridium-end

# Build header
iridium-start :
	@echo "============================="
	@echo "===      IRIDIUM DRV      ==="
	@echo "============================="
	@echo "Files to compile: $(words $(IRIDIUM_SRCS))"
	@echo "Compilation Flags:"
	@echo $(IRIDIUM_CFLAGS)
	@echo "Include Paths:"
	@echo $(IRIDIUM_INCFLAGS)
	@echo "Version Flags:"
	@echo $(VERSION_FLAGS)
	@echo "Start building:"

# Building recipes
$(IRIDIUM_OBJDIR)/%-$(BUILD_TYPE).o : $(IRIDIUM_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(IRIDIUM_CFLAGS) $(IRIDIUM_INCFLAGS) $(VERSION_FLAGS) $< -o $@

# Library generation
$(IRIDIUM_LIB) : $(IRIDIUM_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
iridium-end :
	@echo "Build done"
	@echo ""

# Clean recipe
iridium-clean :
	@echo "Cleaning IRIDIUM build directory ..."
	@rm -rf $(IRIDIUM_OBJDIR)
	@rm -rf $(IRIDIUM_LIB)
	@echo "Done"

endif # BUILD_IRIDIUM_MK #