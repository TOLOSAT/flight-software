# Principal Makefile

##############################################
################ CONFIGURATION ###############
##############################################

# Software Version
MAJOR = 0
MINOR = 2
PATCH = 1

##############################################
################### MAKE #####################
##############################################

.PHONY = all clean echoes

all : build

clean : build-clean

include gen/build.mk
include gen/upload.mk
include gen/configuration.mk
include gen/verification.mk
include gen/help.mk

echoes :
	@echo $(PROJ_NAME)

##############################################
################## COLORS ####################
##############################################

# Colors for terminal output
GREEN = \033[92m
YELLOW = \033[93m
RED = \033[91m
BLUE = \033[94m

# Fonts settings
BOLD = \033[1m
NORMAL = \033[0m
ITALIC = \033[3m
UNDERLINE = \033[4m

# Reset effect
RESET = \033[0m