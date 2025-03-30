# TOLOSAT Autonomous Payload & Avionics Software

## Context

TAPAS (TOLOSAT Autonomous Payload and Avionic Software) is the flight software for the TOLOSAT 3U nanosatellite. This software has different roles including :
- Ensuring the vital functions of the satellite: temperature management, attitude management, memory, power and computing resources management.
- Ensuring satellite - ground communication.
- Ensuring the piloting of the payloads.

If you want to know more about the inner workings of TAPAS, you can read the [technical specifications](doc/technical-specifications/Technical_Specifications.md).

## Requirements

To develop TAPAS, it is necessary to have a LINUX based OS (e.g. Ubuntu, Arch, ...) installed on your computer. The docker allows to avoid compatibility problems between Linux distribution and/or versions.

Windows and MacOS are not recommended for TAPAS development.
For MacOS, it's possible to reproduce the same environment as under Linux, as this is a UNIX OS, and therefore has a high degree of compatibility. However, you need to download all the necessary packages either manually or using HomeBrew (if available), taking care to download the correct versions of some of them.
For Windows, it's possible to reproduce the build environment with WSL, but OpenOCD doesn't work, preventing the use of a physical board. QEMU, on the other hand, works without a problem.

If you have have Docker. Just clone this repository and run the command `./run-docker.sh`. The docker image should be created and then a detached docker should be created. One can either attach VSCode into the container and develop with it, or simply attach the docker to the terminal by doing docker attach {id}. If you are not on Linux you may have trouble with `make upload` that's why Linux is recommended.

If you're on Ubuntu 22.04 and don't want to use Docker, you can install the dependencies for TAPAS installing :
- build-essential
- cppcheck (v2.7 is required)
- kconfig-frontends
- doxygen
- gcc-arm-none-eabi (v10.3.1 is required)
- gdb-multiarch (aliased into arm-none-eabi-gdb)
- git
- graphviz
- nano
- openocd
- telnet
- vim

It is then recommended to download VSCode and the TAPAS extension pack, which can be found at the following address: https://github.com/TOLOSAT/flight-software-extension-pack/tree/main/outputs. Simply download the latest .vsix file and install it with VSCode.

**NOTE :**  Compiling and running outside the docker is possible but deprecated.

## Quick Usage

To quickly use the software, you need to know the following commands:
- `make` or `make all` removes previously generated files, builds the software, and uploads it to the board.
- `make menuconfig` helps you to select the right configuration for your use case
- `make clean` removes all previously generated files.
- `make pre-build` pre-build some sources files based on the csv configuration files.
- `make build` builds the software (without removing files).
- `make upload` retrieves the software image and upload it onto the board.
- `./run-docker.sh` builds and runs the Docker container in the background. You can use the `-a` flag in order to attach automatically.
- `./update-doc.sh` update/creates html documentation with doxygen in the build folder.

Then, if you wish, you can ping the satellite by sending a TC(17,1) : **1855c000000619110100009ccf**

## Environment

TAPAS is based on FreeRTOS and is intended to run on an ARM-M target. In order to be able to develop a software that can use this OS and adapt to several possible targets, it was decided to separate the source code by functionality. We distinguish four sets of functionalities:

- The core corresponds to TAPAS internal API which provide the software management functions.
- The application corresponds to the application code that groups together the satellite's tasks.
- The middleware contains high level drivers or libraries that allow to perform different tasks of the satellite such as communication with payloads or processing of TM and TC.
- The tools are all the layers on which the application is based. They include scripts, the OS, the CMSIS, the BSP and the HALs. The layers contained in tools are obtained from suppliers (ARM, FreeRTOS, ST ...) that's why we defined them as submodules.

In order for each feature to be independent of the others at the time of development but to fit together at the time of compilation we have chosen the following framework:
- TAPAS's features are contained in separate folders.
- TAPAS is based on a set of Makefiles which are responsible for compiling each of the codes and assembling them. These Makefiles are grouped in the gen folder except for the main Makefile which is located at the root.
- No IDE will be used to guarantee the evolution of the code and its porting to several targets and to avoid version compatibility problems.
- A docker containing the compiler and debugger has been created to guarantee the stability of the code and its reproducibility on several machines.
- We chose the arm-none-eabi-gcc compiler version 10.3.1, the debugger is based on openocd version 0.11.0.

## Acronyms

<center>

| Acronym | Definition                                         |
|---------|----------------------------------------------------|
| API     | Application Program Interface                      |
| BSP     | Board Support Package                              |
| CCSDS   | Consultative Commitee for Space Data Systems       |
| CMSIS   | Cortex Microcontroller Software Interface Standard |
| CUC     | CCSDS Usegmented time Code (cf. CCSDS 301.0-B-2)   |
| ECSS    | European Cooperation for Space Standardization     |
| FDIR    | Failure Detection Identification and Recovery      |
| HAL     | Hardware Abstraction Layer                         |
| NVM     | Non Volatile Memory                                |
| OS      | Operating System                                   |
| PUS     | Packet Utilization Standard (cf. ECSS-E-ST-70-41C) |
| PS      | Packet Store                                       |
| RTOS    | Real Time OS                                       |
| TAPAS   | TOLOSAT Autonomous Payload and Avionic Software    |
| TC      | TeleCommand                                        |
| TM      | TeleMetry                                          |

</center>

## Documentation

For more information on TOLOSAT documentation, please consult the technical specifications in the doc folder, or the project doxygen at https://tolosat.github.io/flight-software/.