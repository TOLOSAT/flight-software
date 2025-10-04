# Makefile including all build recipes for kernel third parties

ifndef BUILD_FATFS_MK
BUILD_FATFS_MK := yes

##############################################
############# DIRECTORIES & FILES ############
##############################################

# Directories
FATFS_DIR		= $(THIRD_PARTIES_DIR)/FS/FatFs
FATFS_INCDIR	= $(FATFS_DIR)/source
FATFS_SRCDIR	= $(FATFS_DIR)/source
FATFS_OBJDIR	= $(BUILD_DIR)/third-parties/fatfs

# Files
FATFS_SRCS  = $(filter-out $(FATFS_SRCDIR)/ffsystem_%.c, $(wildcard $(FATFS_SRCDIR)/*.c))
FATFS_OBJS  = $(subst $(FATFS_SRCDIR)/,$(FATFS_OBJDIR)/,$(FATFS_SRCS:.c=.o))
FATFS_LIB   = $(LIBS_DIR)/libfatfs.a

##############################################
#################### FLAGS ###################
##############################################

FATFS_CFLAGS	=	$(CFLAGS) \
					-D$(CHIP) -D$(CHIP_FAMILLY) $(CORE_SELECT) \
					-Wno-unused-variable -Wno-unused-parameter -Wno-stringop-overflow -Wno-unused-function
FATFS_INCDIRS	=	$(FATFS_INCDIR) $(THIRD_PARTIES_CONFDIR) \
					$(CMSIS_INCDIR) $(CMSIS_INCDIR_DEVICE) \
					$(PRE_BUILD_DIR)
FATFS_INCFLAGS	=	$(addprefix -I,$(FATFS_INCDIRS))

##############################################
################ BUILD RECIPES ###############
##############################################

.PHONY : fatfs fatfs-start fatfs-end fatfs-clean
fatfs : fatfs-start $(FATFS_LIB) fatfs-end

# Include dependencies
-include $(FATFS_OBJS:.o=.d)

# Build header
fatfs-start :
	@echo "============================="
	@echo "===         FATFS         ==="
	@echo "============================="
	@echo "Files to compile: $(words $(FATFS_SRCS))"
	@echo "Compilation Flags:"
	@echo $(FATFS_CFLAGS)
	@echo "Include Paths:"
	@$(foreach dir,$(patsubst $(WORKSPACE)/%,%,$(FATFS_INCDIRS)),echo "  - $(dir)";)
	@echo "Start building:"

# Building recipes
$(FATFS_OBJDIR)/%.o : $(FATFS_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(FATFS_CFLAGS) $(FATFS_INCFLAGS) $< -o $@

# Library generation
$(FATFS_LIB) : $(FATFS_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
fatfs-end :
	@echo "Build done"
	@echo ""

# Clean recipe
fatfs-clean :
	@echo "Cleaning FATFS build directory ..."
	@rm -rf $(FATFS_OBJDIR)
	@rm -rf $(FATFS_LIB)
	@echo "Done"

endif # BUILD_FATFS_MK #