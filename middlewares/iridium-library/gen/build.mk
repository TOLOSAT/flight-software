# Makefile including all build recipes

ifndef BUILD_MK
BUILD_MK := yes

##############################################
############# DIRECTORIES & FILES ############
##############################################

# Directories
INCDIR	= inc
SRCDIR	= src
OBJDIR	= $(BUILD_DIR)/middlewares/$(LIB_NAME)
LIBDIR  = $(BUILD_DIR)/libs

# Files
SRCS = $(wildcard $(SRCDIR)/*.c $(SRCDIR)/*/*.c)
OBJS = $(subst $(SRCDIR)/,$(OBJDIR)/,$(SRCS:.c=.o))
LIB  = $(LIBDIR)/lib$(LIB_NAME).a

##############################################
#################### FLAGS ###################
##############################################

CFLAGS   += $(CFLAGS)
INCFLAGS += -I$(INCDIR) -I$(KERNEL_HEADERS) $(addprefix -I,$(EXTRA_INCS))

##############################################
################ BUILD RECIPES ###############
##############################################

.PHONY : build start end clean
build: start $(LIB) end

# Include dependencies
-include $(OBJS:.o=.d)

# Build header
start :
	@echo "============================="
	@echo "======     LIBRARY     ======"
	@echo "============================="
	@echo "Library name: $(LIB_NAME)"
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
	@$(CC) $(CFLAGS) $(INCFLAGS) $< -o $@

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
	@echo "Cleaning $(LIB_NAME) build directory ..."
	@rm -rf $(OBJDIR)
	@rm -rf $(LIB)
	@echo "Done"

endif # BUILD_MK #