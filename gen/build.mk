# Software Building Makefile

include gen/pre-build.mk
include gen/build_core.mk
include gen/build_application.mk
include gen/build_os.mk
include gen/build_middlewares.mk
include gen/build_hal.mk
include gen/build_bsp.mk
include gen/build_boot.mk

##############################################
#################### BUILD ###################
##############################################

# Targets definitions
TARGET_DBG		= $(TARGET_DIR)/$(PROJ_NAME)-$(VERSION).elf
TARGET_RLS		= $(TARGET_DIR)/$(PROJ_NAME)-$(VERSION).elf
BOOT_TARGET_DBG	= $(TARGET_DIR)/boot-software-$(VERSION).elf
BOOT_TARGET_RLS	= $(TARGET_DIR)/boot-software-$(VERSION).elf

# Target definition according to version
ifeq ($(VERSION), debug)
TARGET 			= $(TARGET_DBG)
BOOT_TARGET 	= $(BOOT_TARGET_DBG)
VERSION_FLAGS 	= $(DEBUG_FLAGS)
else ifeq ($(VERSION), release)
TARGET 			= $(TARGET_RLS)
BOOT_TARGET 	= $(BOOT_TARGET_RLS)
VERSION_FLAGS 	= $(RELEASE_FLAGS)
else
$(error Please select debug or release)
endif

##############################################
######## SOFTWARE BUILD CONFIGURATION ########
##############################################

PRIVATE_COMPONENTS = application core pus time tolosat-fs iridiumdrv generic-hal bsp
PUBLIC_COMPONENTS = os hal fatfs

PRIVATE_LIBS = $(foreach lib,$(PRIVATE_COMPONENTS),-l$(lib)-$(VERSION))
PUBLIC_LIBS = $(foreach lib,$(PUBLIC_COMPONENTS),-l$(lib)-$(VERSION))

##############################################
########## BOOT BUILD CONFIGURATION ##########
##############################################

BOOT_PRIVATE_COMPONENTS = bootcore bsp
BOOT_PUBLIC_COMPONENTS = hal fatfs

BOOT_PRIVATE_LIBS = $(foreach lib,$(BOOT_PRIVATE_COMPONENTS),-l$(lib)-$(VERSION))
BOOT_PUBLIC_LIBS = $(foreach lib,$(BOOT_PUBLIC_COMPONENTS),-l$(lib)-$(VERSION))

##############################################
################ BUILD RECIPE ################
##############################################

.PHONY += build boot

build : $(TARGET)

boot : $(BOOT_TARGET)

# Target Linking Stage
$(TARGET) : $(PUBLIC_COMPONENTS) $(PRIVATE_COMPONENTS)
	mkdir -p $(@D)
	$(CC) -L$(BUILD_LIBS_DIR) -Wl,--whole-archive $(PRIVATE_LIBS) -Wl,--no-whole-archive $(PUBLIC_LIBS) $(PROJECT_LDFLAGS) -T $(LINKER_SCRIPT) -o $@ > $(@:.elf=.size)
	$(READELF) -a $@ > $(@:.elf=.readelf)
	@echo "*****************************"
	@echo "***   Target Build Done   ***"
	@echo "*****************************"

$(BOOT_TARGET) : $(BOOT_PUBLIC_COMPONENTS) $(BOOT_PRIVATE_COMPONENTS)
	mkdir -p $(@D)
	$(CC) -L$(BUILD_LIBS_DIR) -Wl,--whole-archive $(BOOT_PRIVATE_LIBS) -Wl,--no-whole-archive $(BOOT_PUBLIC_LIBS) $(PROJECT_LDFLAGS) -T $(LINKER_SCRIPT:$(LOAD_MEMORY).ld=boot.ld) -o $@ > $(@:.elf=.size)
	$(READELF) -a $@ > $(@:.elf=.readelf)
	@echo "*****************************"
	@echo "****   Boot Build Done   ****"
	@echo "*****************************"