# Makefile incluant les parametres d'envirronement

include conf/board_settings.mk 
include conf/cc_settings.mk
include conf/debug_settings.mk

##############################################
################### TOOLS ####################
##############################################

# Tools
CC = /usr/bin/arm-none-eabi-gcc
GDB = /usr/bin/gdb-multiarch
OCD = /usr/bin/openocd
