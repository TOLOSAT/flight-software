# HAL Building Makefile

##############################################
#################### HAL #####################
##############################################

# HAL flags
HAL_CFLAGS    = $(PROJECT_CFLAGS) -Wno-unused-variable -Wno-unused-parameter
HAL_INCFLAGS  = -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
HAL_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)

# HAL files
include $(HAL_SRCS_LIST)
HAL_OBJS  = $(subst $(HAL_SRCDIR)/,$(HAL_OBJDIR)/,$(HAL_SRCS:.c=-$(BUILD_TYPE).o))
HAL_LIB   = $(BUILD_LIBS_DIR)/libhal-$(BUILD_TYPE).a

# Include dependencies
-include $(HAL_OBJS:.o=.d)

# HAL recipes
hal : hal-start $(HAL_LIB) hal-end

# Build header
hal-start :
	@echo "============================="
	@echo "===          HAL          ==="
	@echo "============================="
	@echo "Files to compile: $(words $(HAL_SRCS))"
	@echo "Compilation Flags:"
	@echo $(HAL_CFLAGS)
	@echo "Include Paths:"
	@echo $(HAL_INCFLAGS)
	@echo "Version Flags:"
	@echo $(VERSION_FLAGS)
	@echo "Start building:"

# Building recipes
$(HAL_OBJDIR)/%-$(BUILD_TYPE).o : $(HAL_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(HAL_CFLAGS) $(HAL_INCFLAGS) $(VERSION_FLAGS) $< -o $@ 

# Library generation
$(HAL_LIB) : $(HAL_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
hal-end :
	@echo "Build done"
	@echo ""

##############################################
################ FATS LIBRARY ################
##############################################

# FATFS flags
FATFS_CFLAGS    = $(PROJECT_CFLAGS) -Wno-unused-variable -Wno-unused-parameter -Wno-stringop-overflow -Wno-unused-function
FATFS_INCFLAGS  = -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
FATFS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)

# FATFS files
FATFS_SRCS  = $(wildcard $(FATFS_SRCDIR)/*.c)
FATFS_OBJS  = $(subst $(FATFS_SRCDIR)/,$(FATFS_OBJDIR)/,$(FATFS_SRCS:.c=-$(BUILD_TYPE).o))
FATFS_LIB   = $(BUILD_LIBS_DIR)/libfatfs-$(BUILD_TYPE).a

# Include dependencies
-include $(FATFS_OBJS:.o=.d)

# FATFS recipes
fatfs : fatfs-start $(FATFS_LIB) fatfs-end

# Build header
fatfs-start :
	@echo "============================="
	@echo "===         FATFS         ==="
	@echo "============================="
	@echo "Files to compile: $(words $(FATFS_SRCS))"
	@echo "Compilation Flags:"
	@echo $(FATFS_CFLAGS)
	@echo "Include Paths:"
	@echo $(FATFS_INCFLAGS)
	@echo "Version Flags:"
	@echo $(VERSION_FLAGS)
	@echo "Start building:"

# Building recipes
$(FATFS_OBJDIR)/%-$(BUILD_TYPE).o : $(FATFS_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(FATFS_CFLAGS) $(FATFS_INCFLAGS) $(VERSION_FLAGS) $< -o $@ 

# Library generation
$(FATFS_LIB) : $(FATFS_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# Build footer
fatfs-end :
	@echo "Build done"
	@echo ""
