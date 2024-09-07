# HAL Building Makefile

##############################################
#################### HAL #####################
##############################################

# HAL Flags
HAL_CFLAGS    = $(PROJECT_CFLAGS) -Wno-unused-variable -Wno-unused-parameter
HAL_INCFLAGS  = -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy -I$(CONF_HALS_DIR)
HAL_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)

# HAL Files
include $(HAL_SRCS_LIST)
HAL_OBJS  = $(subst $(HAL_SRCDIR)/,$(HAL_OBJDIR)/,$(HAL_SRCS:.c=-$(BUILD_TYPE).o))
HAL_LIB   = $(BUILD_LIBS_DIR)/libhal-$(BUILD_TYPE).a

# Include dependancies
-include $(HAL_OBJS:.o=.d)

# HAL compilation
$(HAL_OBJDIR)/%-$(BUILD_TYPE).o : $(HAL_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(HAL_CFLAGS) $(HAL_INCFLAGS) $(VERSION_FLAGS) $< -o $@ 

# HAL Library
$(HAL_LIB) : $(HAL_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# HAL Recipe
hal-start :
	@echo "**************************************"
	@echo "*********   HAL Start Build   ********"
	@echo "**************************************"

hal-end :
	@echo "**************************************"
	@echo "*********   HAL Build Done   *********"
	@echo "**************************************"
	@echo

hal : hal-start $(HAL_LIB) hal-end

##############################################
################ FATS LIBRARY ################
##############################################

# FATFS Flags
FATFS_CFLAGS    = $(PROJECT_CFLAGS) -Wno-unused-variable -Wno-unused-parameter -Wno-stringop-overflow -Wno-unused-function
FATFS_INCFLAGS  = -I$(FATFS_INCDIR) -I$(CONF_FATFS_DIR)
FATFS_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE)

# FATFS Files
FATFS_SRCS  = $(wildcard $(FATFS_SRCDIR)/*.c)
FATFS_OBJS  = $(subst $(FATFS_SRCDIR)/,$(FATFS_OBJDIR)/,$(FATFS_SRCS:.c=-$(BUILD_TYPE).o))
FATFS_LIB   = $(BUILD_LIBS_DIR)/libfatfs-$(BUILD_TYPE).a

# Include dependancies
-include $(FATFS_OBJS:.o=.d)

# FATFS compilation
$(FATFS_OBJDIR)/%-$(BUILD_TYPE).o : $(FATFS_SRCDIR)/%.c
	@echo "  CC  $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(FATFS_CFLAGS) $(FATFS_INCFLAGS) $(VERSION_FLAGS) $< -o $@ 

# FATFS Library
$(FATFS_LIB) : $(FATFS_OBJS)
	@echo "  AR  $(@F)"
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

# FATFS Recipe
fatfs-start :
	@echo "**************************************"
	@echo "********   FATFS Start Build   *******"
	@echo "**************************************"

fatfs-end :
	@echo "**************************************"
	@echo "********   FATFS Build Done   ********"
	@echo "**************************************"
	@echo

fatfs : fatfs-start $(FATFS_LIB) fatfs-end
