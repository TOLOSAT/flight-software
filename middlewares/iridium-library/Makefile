# IRIDIUM Building Makefile

##############################################
################ REQUIREMENTS ################
##############################################

BUILD_DIR 			?= $(error BUILD_DIR is required)
CC 		  			?= $(error CC is required)
CFLAGS 	  			?= $(error CFLAGS is required)
KERNEL_HEADERS		?= $(error KERNEL_HEADERS is required)
PRE_BUILD_HEADERS	?= $(error PRE_BUILD_HEADERS is required)
PUS_HEADERS			?= $(error PUS_HEADERS is required)

##############################################
############# IRIDIUM DIRECTORIES ############
##############################################

# Directories
INCDIR	= inc
SRCDIR	= src
OBJDIR	= $(BUILD_DIR)/middlewares/iridium
LIB_DIR = $(BUILD_DIR)/libs

##############################################
############### IRIDIUM LIBRARY ##############
##############################################

# Iridium driver files
SRCS = $(wildcard $(SRCDIR)/*.c $(SRCDIR)/*/*.c)
OBJS = $(subst $(SRCDIR)/,$(OBJDIR)/,$(SRCS:.c=.o))
LIB  = $(LIB_DIR)/libiridium.a

# Iridium driver flags
CFLAGS   += $(CFLAGS)
INCFLAGS += -I$(INCDIR) -I$(KERNEL_HEADERS) -I$(PRE_BUILD_HEADERS) -I$(PUS_HEADERS)

# Include dependencies
-include $(OBJS:.o=.d)

# Iridium Driver recipes
.PHONY : all start end clean
all : start $(LIB) end

# Build header
start :
	@echo "============================="
	@echo "===      IRIDIUM LIB      ==="
	@echo "============================="
	@echo "Files to compile: $(words $(SRCS))"
	@echo "Compilation Flags:"
	@echo $(CFLAGS)
	@echo "Include Paths:"
	@echo $(INCFLAGS)
	@echo "Start building:"

# Building recipes
$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(VERSION_FLAGS) $(INCFLAGS) $< -o $@

# Library generation
$(LIB) : $(OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
end :
	@echo "Build done"
	@echo ""

# Clean recipe
clean :
	@echo "Cleaning IRIDIUM build directory ..."
	@rm -rf $(OBJDIR)
	@rm -rf $(LIB)
	@echo "Done"
