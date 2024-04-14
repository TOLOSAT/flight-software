#!/bin/bash

# Menu Variables 
CURR_OPTION="1"

# Configuration Variables
PROJ_NAME="flight-software"
VERSION=""
BOARD=""
LOAD_MEMORY=""
CONSOLE_MODE=""
FS_MODE=""
TEST=""

# First load the project configuration if a config file exists
if [ -f ".config" ]; then
    while IFS='=' read -r key value; do
        case "$key" in
            "PROJ_NAME") PROJ_NAME=$value ;;
            "VERSION") VERSION=$value ;;
            "BOARD") BOARD=$value ;;
            "LOAD_MEMORY") LOAD_MEMORY=$value ;;
            "CONSOLE_MODE") CONSOLE_MODE=$value ;;
            "FS_MODE") FS_MODE=$value ;;
            "TEST") TEST=$value ;;
        esac
    done < ".config"
fi

# Configuration Menu
while true; do
    EXEC=$(dialog --clear --backtitle "TAPAS Configuration" \
        --title "Main Menu" \
        --ok-label "Select" \
        --extra-button --extra-label "Save" \
        --cancel-label "Exit" \
        --default-item "$CURR_OPTION" \
        --menu "Please configure the flight software using the arrows and \
                the enter key. Use <Save> button to save your changes at any time." 18 65 5 \
        "1" "Project Name ($PROJ_NAME)" \
        "2" "Version ($VERSION)" \
        "3" "Target Board ($BOARD)" \
        "4" "Memory Load ($LOAD_MEMORY)" \
        "5" "Console Mode ($CONSOLE_MODE)" \
        "6" "File System Mode ($FS_MODE)" \
        "7" "Target Test ($TEST)" \
        3>&1 1>&2 2>&3 3>&- )

    # Check if Save was pressed
    if [ $? -eq 3 ]; then
        # Save configurations to a new file
        echo "# TAPAS Configuration File" > .config
        echo "PROJ_NAME=$PROJ_NAME" >> .config
        echo "VERSION=$VERSION" >> .config
        echo "BOARD=$BOARD" >> .config
        echo "LOAD_MEMORY=$LOAD_MEMORY" >> .config
        echo "CONSOLE_MODE=$CONSOLE_MODE" >> .config
        echo "FS_MODE=$FS_MODE" >> .config
        echo "TEST=$TEST" >> .config
        dialog --title "Configuration Save" --msgbox "Configurations saved successfully!" 6 60
        continue
    fi

    # Update current option in the menu
    CURR_OPTION=$EXEC

    # Then goes to the chosen submenu
    case $EXEC in
        1)
            # Menu for project name selection
            PROJ_NAME=$(dialog --stdout --backtitle "TAPAS Configuration" \
                               --title "Project Name" \
                               --inputbox "Enter the new project name:" 8 50 "$PROJ_NAME")
            if [ $? -ne 0 ]; then
                PROJ_NAME=""
            fi
            ;;
        2)
            # Menu for version selection
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
            # Menu for board selection
            EXEC=$(dialog --clear --backtitle "TAPAS Configuration" \
                --title "Board Selection" \
                --ok-label "Select" \
                --cancel-label "Exit" \
                --menu "Please select the target board :" 15 65 4 \
                "1" "ART Pi" \
                "2" "Nucleo H745ZI" \
                "3" "Nucleo F411RE" \
                "4" "Discovery F407VG" \
                3>&1 1>&2 2>&3 3>&- )
            case $EXEC in
                1)
                    BOARD="ART_PI"
                    ;;
                2)
                    BOARD="NUCLEO_H745ZI"
                    ;;
                3)
                    BOARD="NUCLEO_F411RE"
                    ;;
                4)
                    BOARD="DISCOVERY_F407VG"
                    ;;
            esac
            ;;
        4)
            # Menu for load memory selection
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
                    LOAD_MEMORY="RAM"
                    ;;
                2)
                    LOAD_MEMORY="FLASH"
                    ;;
            esac
            ;;
        5)
            # Menu for console mode selection
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
                    CONSOLE_MODE="NONE"
                    ;;
                2)
                    CONSOLE_MODE="UART"
                    ;;
                3)
                    CONSOLE_MODE="FILE"
                    ;;
                4)
                    CONSOLE_MODE="CIRCULAR-BUFFER"
                    ;;
            esac
            ;;
        6)
            # Menu for file system mode selection
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
                    FS_MODE="NONE"
                    ;;
                2)
                    FS_MODE="SPI"
                    ;;
                3)
                    FS_MODE="SDMMC"
                    ;;
            esac
            ;;
        7)
            # Menu for test selection
            TEST=$(dialog --stdout --backtitle "TAPAS Configuration" \
                               --title "Target Test" \
                               --inputbox "Enter the test name. Leave empty if you want to run the full flight software" 8 50 "$TEST")
            if [ $? -ne 0 ]; then
                TEST=""
            fi
            ;;
        *)
            break
            ;;
    esac
done

clear