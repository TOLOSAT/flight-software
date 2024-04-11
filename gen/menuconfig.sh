#!/bin/bash

# Variables initialisation
TARGET_BOARD=""
VERSION=""
MEMORY=""
CONSOLE_MODE=""
FS_MODE=""

while true; do
    EXEC=$(dialog --clear --backtitle "TAPAS Configuration" \
        --title "Menu Principal" \
        --ok-label "Select" \
        --cancel-label "Exit" \
        --menu "Please configure the flight software using the arrows and \
                the enter key. When you have finished configuring use <Save> \
                to save your changes." 18 65 5 \
        "1" "Target Board ($TARGET_BOARD) --->" \
        "2" "Version ($VERSION) --->" \
        "3" "Memory Load ($MEMORY) --->" \
        "4" "Console Mode ($CONSOLE_MODE) --->" \
        "5" "File System Mode ($FS_MODE) --->" \
        3>&1 1>&2 2>&3 3>&- )

    case $EXEC in
        1)
            EXEC=$(dialog --clear --backtitle "TAPAS Configuration" \
                --title "Board Selection" \
                --ok-label "Select" \
                --cancel-label "Exit" \
                --menu "Please select the target board :" 15 65 4 \
                "1" "ART PI" \
                "2" "Nucleo H745ZI" \
                "3" "Nucleo F411RE" \
                "4" "Discovery F407VG" \
                3>&1 1>&2 2>&3 3>&- )
            case $EXEC in
                1)
                    TARGET_BOARD="ART_PI"
                    ;;
                2)
                    TARGET_BOARD="NUCLEO_H745ZI"
                    ;;
                3)
                    TARGET_BOARD="NUCLEO_F411RE"
                    ;;
                4)
                    TARGET_BOARD="DISCOVERY_F407VG"
                    ;;
            esac
            ;;
        2)
            EXEC=$(dialog --clear --backtitle "TAPAS Configuration" \
                --title "Version Selection" \
                --ok-label "Select" \
                --cancel-label "Exit" \
                --menu "Please select the software version :" 10 65 2 \
                "1" "Debug" \
                "2" "Release" \
                3>&1 1>&2 2>&3 3>&- )
            case $EXEC in
                1)
                    VERSION="debug"
                    ;;
                2)
                    VERSION="release"
                    ;;
            esac
            ;;
        3)
            EXEC=$(dialog --clear --backtitle "TAPAS Configuration" \
                --title "Load Memory Selection" \
                --ok-label "Select" \
                --cancel-label "Exit" \
                --menu "Please select the load memory :" 10 65 2 \
                "1" "RAM" \
                "2" "FLASH" \
                3>&1 1>&2 2>&3 3>&- )
            case $EXEC in
                1)
                    MEMORY="ram"
                    ;;
                2)
                    MEMORY="flash"
                    ;;
            esac
            ;;
        4)
            EXEC=$(dialog --clear --backtitle "TAPAS Configuration" \
                --title "Console Selection" \
                --ok-label "Select" \
                --cancel-label "Exit" \
                --menu "Please select the console mode :" 15 65 4 \
                "1" "None" \
                "2" "UART" \
                "3" "File" \
                "4" "Circular Buffer" \
                3>&1 1>&2 2>&3 3>&- )
            case $EXEC in
                1)
                    CONSOLE_MODE="none"
                    ;;
                2)
                    CONSOLE_MODE="uart"
                    ;;
                3)
                    CONSOLE_MODE="file"
                    ;;
                4)
                    CONSOLE_MODE="circular-buffer"
                    ;;
            esac
            ;;
        5)
            EXEC=$(dialog --clear --backtitle "TAPAS Configuration" \
                --title "File System Selection" \
                --ok-label "Select" \
                --cancel-label "Exit" \
                --menu "Please select the file system mode :" 15 65 3 \
                "1" "None" \
                "2" "SPI" \
                "3" "SDMMC" \
                3>&1 1>&2 2>&3 3>&- )
            case $EXEC in
                1)
                    FS_MODE="none"
                    ;;
                2)
                    FS_MODE="spi"
                    ;;
                3)
                    FS_MODE="sdmmc"
                    ;;
            esac
            ;;
        *)
            break
            ;;
    esac
done

clear