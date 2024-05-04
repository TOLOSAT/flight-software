# Software Building Makefile

include gen/pre-build.mk
include gen/build_core.mk
include gen/build_hal.mk
include gen/build_bsp.mk

##############################################
#################### BUILD ###################
##############################################

# Targets definitions
TARGET_DBG = $(TARGET_DIR)/boot-software-$(VERSION).elf
TARGET_RLS = $(TARGET_DIR)/boot-software-$(VERSION).elf

# Target definition according to version
ifeq ($(VERSION), debug)
TARGET 			= $(TARGET_DBG)
VERSION_FLAGS 	= $(DEBUG_FLAGS)
else ifeq ($(VERSION), release)
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

PRIVATE_LIBS = $(foreach lib,$(PRIVATE_COMPONENTS),-l$(lib)-$(VERSION))
PUBLIC_LIBS = $(foreach lib,$(PUBLIC_COMPONENTS),-l$(lib)-$(VERSION))

##############################################
################ BUILD RECIPE ################
##############################################

.PHONY += build

build : $(TARGET)

# Target Linking Stage
$(TARGET) : $(PRIVATE_COMPONENTS) $(PUBLIC_COMPONENTS)
	mkdir -p $(@D)
	$(CC) -L$(BUILD_LIBS_DIR) -Wl,--whole-archive $(PRIVATE_LIBS) -Wl,--no-whole-archive $(PUBLIC_LIBS) $(PROJECT_LDFLAGS) -T $(LD_SCRIPT) -o $@ > $(@:.elf=.size)
	$(READELF) -a $@ > $(@:.elf=.readelf)
	@echo "*****************************"
	@echo "***   Target Build Done   ***"
	@echo "*****************************"