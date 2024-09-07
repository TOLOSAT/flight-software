# Software Building Makefile

include gen/pre-build.mk
include gen/build_core.mk
include gen/build_third_parties.mk
include gen/build_bsp.mk

##############################################
#################### BUILD ###################
##############################################

# Targets definitions
TARGET_DBG = $(TARGET_DIR)/boot-software-$(BUILD_TYPE).elf
TARGET_RLS = $(TARGET_DIR)/boot-software-$(BUILD_TYPE).elf

# Target definition according to version
ifeq ($(BUILD_TYPE), debug)
TARGET 			= $(TARGET_DBG)
VERSION_FLAGS 	= $(DEBUG_FLAGS)
else ifeq ($(BUILD_TYPE), release)
TARGET 			= $(TARGET_RLS)
VERSION_FLAGS 	= $(RELEASE_FLAGS)
else
$(error Please select debug or release)
endif

##############################################
######## SOFTWARE BUILD CONFIGURATION ########
##############################################

PRIVATE_COMPONENTS = core bsp
PUBLIC_COMPONENTS = hal fatfs

PRIVATE_LIBS = $(foreach lib,$(PRIVATE_COMPONENTS),-l$(lib)-$(BUILD_TYPE))
PUBLIC_LIBS = $(foreach lib,$(PUBLIC_COMPONENTS),-l$(lib)-$(BUILD_TYPE))

##############################################
################ BUILD RECIPE ################
##############################################

.PHONY += build

build : $(TARGET)

# Target Linking Stage
$(TARGET) : $(PRIVATE_COMPONENTS) $(PUBLIC_COMPONENTS)
	@echo "**************************************"
	@echo "*******   TARGET Start Build   *******"
	@echo "**************************************"
	@echo "  LD  $(@F)"
	@mkdir -p $(@D)
	@$(CC) -L$(BUILD_LIBS_DIR) -Wl,--whole-archive $(PRIVATE_LIBS) -Wl,--no-whole-archive $(PUBLIC_LIBS) $(PROJECT_LDFLAGS) -T $(LD_SCRIPT) -o $@ > $(@:.elf=.size)
	@$(READELF) -a $@ > $(@:.elf=.readelf)
	@echo "**************************************"
	@echo "********   TARGET Build Done   *******"
	@echo "**************************************"
	@echo