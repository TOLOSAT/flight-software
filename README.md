# TOLOSAT Autonomous Payload & Avionics Software (TAPAS)

## 1. Overview

**TAPAS** (TOLOSAT Autonomous Payload and Avionic Software) is the flight software framework developed for the **TOLOSAT 3U nanosatellite**. It provides the necessary software infrastructure to ensure autonomous satellite operation, ground communication, and payload management.

TAPAS is designed as a **modular and reusable software framework** composed of multiple components that interact through well-defined interfaces. Its layered structure allows for maintainability, portability, and reuse across future nanosatellite missions.

The main objectives of TAPAS are:
- **Autonomous satellite management**: control of thermal, attitude, power, and data subsystems.
- **Ground communication**: handling of telecommands (TC) and telemetry (TM) through standardized ECSS-PUS protocols.
- **Payload operations**: autonomous and remote operation of scientific and communication payloads.

## 2. Software Architecture

The TAPAS software stack is divided into three main components:
- **Kernel** : Core of the system providing the operating system services such as multitasking, memory management, fault handling (FDIR), peripheral abstraction, and file system access.
- **Middlewares** : Reusable software libraries built on top of the kernel. They include the PUS library (ECSS-compliant TM/TC system) and mission-specific libraries (e.g., Iridium communications).
- **Applications** : Mission-level subsystems implementing TOLOSAT-specific behavior (AOCS, thermal control, power management, gravimetry, Iridium payload, TM/TC management).

## 3. Development Environment

A **Linux-based system** is required (Ubuntu 22.04 recommended). Development under macOS or Windows (via WSL) is possible but not officially supported.

### 3.1 Docker Environment (Recommended)

A Docker container is provided to ensure a consistent and reproducible environment across all development machines.

```bash
git clone https://github.com/TOLOSAT/flight-software.git
cd flight-software
./run-docker.sh
```

This command builds and runs the TAPAS Docker image in the background. You can then attach VSCode to the container or connect manually using:

```bash
docker ps
docker attach <container_id>
```

### 3.2 Native Installation (Ubuntu 22.04)

If you prefer not to use Docker, install the following dependencies:

```bash
sudo apt install build-essential cppcheck kconfig-frontends doxygen \
  gcc-arm-none-eabi gdb-multiarch git graphviz nano openocd telnet vim
```

**Required versions:**
- `cppcheck` >= 2.7
- `gcc-arm-none-eabi` = 10.3.1
- `openocd` = 0.11.0

It is also recommended to install the **TAPAS VSCode Extension Pack**:
[https://github.com/TOLOSAT/flight-software-extension-pack/tree/main/outputs](https://github.com/TOLOSAT/flight-software-extension-pack/tree/main/outputs)

> Building and running outside Docker is possible but deprecated.

## 4. Build System and Usage

The build system is based on a hierarchy of Makefiles and a Kconfig configuration layer.

Common commands:

| Command                 | Description                                                |
|-------------------------|------------------------------------------------------------|
| `make` / `make all`     | Cleans, builds, and uploads the flight software image.     |
| `make build`            | Builds without cleaning.                                   |
| `make clean`            | Removes generated files.                                   |
| `make menuconfig`       | Opens configuration menu (Kconfig-based).                  |
| `make pre-build`        | Generates precompiled source files based on configuration. |
| `make upload`           | Uploads the compiled image to the target board.            |
| `./run-docker.sh [-a]`  | Builds and runs Docker container (`-a` to attach).         |
| `./tools/update-doc.sh` | Generates Doxygen HTML documentation.                      |

Example test telecommand (using a serial connection):
```
1855c000000619110100009ccf   # TC(17,1): ping command
```

## 5. Coding Standards and Quality Assurance

TAPAS software is written in **C** and follows best practices from the aerospace and embedded software domains:

- Compliance with **MISRA-C:2012** guidelines.
- Static analysis using **cppcheck**.
- Automatic documentation generation with **Doxygen**.
- Enforced code style via **clang-format**.
- Version control using **Git** with feature/fix branching and peer review.
- Partial compliance with **ECSS-E-ST-40C** (Software Engineering) and **ECSS-Q-ST-80C** (Software Product Assurance).

## 6. Supported Platforms

The TAPAS kernel and applications can be built for multiple hardware targets using dedicated configuration files located in `configs/`:
- **Art-Pi (default)**
- **Klara board**
- **Nucleo-F411RE**
- **Nucleo-H745ZI**
- **QEMU (simulation)**

Switch target using:
```bash
make menuconfig
```

## 7. Documentation

- Online documentation: [https://tolosat.github.io/flight-software](https://tolosat.github.io/flight-software)
- Technical note: *TOLOSAT_TN_13_25 – TAPAS Flight Software Architecture and Kernel Description*
- ECSS Standards:
  - ECSS-E-ST-40C — Software Engineering
  - ECSS-Q-ST-80C — Software Product Assurance
  - ECSS-E-ST-70-41C — Packet Utilization Standard (PUS)
  - Online documentation: [https://tolosat.github.io/flight-software](https://tolosat.github.io/flight-software)
- SAVOIR Standards

## 8. Acronyms

| Acronym | Definition                                         |
|---------|----------------------------------------------------|
| API     | Application Programming Interface                  |
| AOCS    | Attitude and Orbit Control System                  |
| BSP     | Board Support Package                              |
| CCSDS   | Consultative Committee for Space Data Systems      |
| CMSIS   | Cortex Microcontroller Software Interface Standard |
| CUC     | CCSDS Unsegmented Time Code                        |
| ECSS    | European Cooperation for Space Standardization     |
| EPS     | Electrical Power System                            |
| FDIR    | Failure Detection, Isolation and Recovery          |
| HAL     | Hardware Abstraction Layer                         |
| NVM     | Non-Volatile Memory                                |
| OS      | Operating System                                   |
| PUS     | Packet Utilization Standard                        |
| PS      | Packet Store                                       |
| RTOS    | Real-Time Operating System                         |
| TAPAS   | TOLOSAT Autonomous Payload and Avionic Software    |
| TC      | TeleCommand                                        |
| TM      | TeleMetry                                          |

## 9. Authors and Maintainers

- **Merlin Kooshmanian** — Flight Software Architect

For technical questions or contributions, please contact the TOLOSAT Flight Software team.
