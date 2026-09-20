# TOLOSAT Flight Software

The TOLOSAT flight software is the onboard software developed for the TOLOSAT 3U CubeSat. It is built on TAPAS (TAPAS Adaptative Platform for Avionic Software), the reusable avionics framework and kernel targeting ARM Cortex-M microcontrollers. This repository integrates TAPAS with middleware libraries and TOLOSAT mission applications.

## Architecture

The repository is split into three software layers:

- `kernel/`: the TAPAS kernel, providing operating-system services, hardware abstraction, file-system integration, monitoring, FDIR, and platform support;
- `middlewares/`: reusable PUS, Iridium, and thermal libraries;
- `applications/`: TOLOSAT mission tasks and subsystem logic.

The source trees follow the same visibility convention:

```text
<module>/
├── include/                  public API exported outside the module
└── components/
    └── <component>/
        ├── <header>.h        API shared with other components of the module
        ├── inc/              private headers, when required
        └── src/              implementation
```

Applications currently expose no public API outside their layer, so their headers remain private under `applications/components/*/inc`.

Other important directories are:

| Path | Purpose |
|------|---------|
| `configs/` | Top-level Kconfig defconfig files. `default_defconfig` is a symbolic link to the default board configuration. |
| `gen/` | Makefiles, Kconfig description, Doxygen configuration, and verification settings. |
| `tools/` | Code-generation, formatting, documentation, debug, and utility scripts. |
| `build/` | Generated files, objects, libraries, build-state descriptions, and final images. This directory is not versioned. |

The kernel and middleware libraries are Git submodules. Clone them together with the main repository:

```bash
git clone --recurse-submodules https://github.com/TOLOSAT/flight-software.git
cd flight-software
```

For an existing checkout, initialise or refresh them with:

```bash
git submodule update --init --recursive
```

## Development environment

The supported development environment is the provided Ubuntu 22.04 Docker image. It pins the versions used by CI, including GCC Arm Embedded 10.3.1, Cppcheck 2.7, and clang-format 19.

Start the container in the background:

```bash
./run-docker.sh
```

Then attach a shell:

```bash
./run-docker.sh --attach
```

Use `./run-docker.sh --help` for image update and container management options. Native builds are possible when the same tools are installed, but are not the reference workflow.

## Configuration and build

Load the default configuration before the first build:

```bash
make default_defconfig
make -j"$(nproc)"
```

The default configuration currently targets ART-Pi. Other configurations can be selected explicitly, for example:

```bash
make qemu_defconfig
make nucleo_f411re_defconfig
```

`make menuconfig` exposes the project options and those of all configurable modules in a single interface, storing the complete firmware configuration in `.config`. A module may also provide standalone `menuconfig` and defconfig workflows; when supported, run Make from that module's directory to manage its independent `.config`.

Common commands are:

| Command | Description |
|---------|-------------|
| `make`, `make all`, `make build` | Incrementally build the complete firmware. |
| `make clean` | Remove the complete `build/` directory. |
| `make <module>` | Build one module, such as `kernel`, `pus`, `thermal`, or `applications`. |
| `make <module>-clean` | Clean one module. |
| `make print-<variable>` | Print an effective Make variable for diagnostics. |
| `make verif` | Run Cppcheck static analysis. |
| `make format` | Apply the repository clang-format configuration. |
| `make doc` | Generate Doxygen documentation under `build/doxygen/`. |
| `make upload` | Run the upload or emulator flow selected by the configuration. |
| `make help` | Display the complete target summary. |

The final ELF and its reports are written to `build/output/`. Static libraries are written to `build/libs/`. Files under `build/state/*.flags` record the effective compiler, flags, include paths, and sources for each build domain; they allow Make to invalidate only the affected archive when build settings change.

## Supported targets

- ART-Pi (default)
- KLARA
- Nucleo-F411RE
- Nucleo-H745ZI
- QEMU CMSDK simulation

Board-specific Kconfig options, linker scripts, generated peripheral descriptions, startup code, and BSP sources are located under `kernel/bsp/`.

## Quality and contribution

The project uses MISRA C:2012-oriented static analysis, compiler warnings as errors, clang-format, and Doxygen checks. See [`CODING_GUIDELINES.md`](CODING_GUIDELINES.md) before contributing. CI builds the default configuration and checks static analysis, formatting, and documentation.

## License

TOLOSAT-developed code is licensed under the Apache License 2.0. See
[`LICENSE`](LICENSE) for details. Third-party components remain subject to
their respective licenses.

## Documentation

- Generated API documentation: <https://tolosat.github.io/flight-software>
- Technical note: *TOLOSAT_TN_13_25 – TAPAS Flight Software Architecture and Kernel Description*
- Reference standards: ECSS-E-ST-40C, ECSS-Q-ST-80C, ECSS-E-ST-70-41C, and SAVOIR

## Maintainers

- Merlin Kooshmanian — Flight Software Architect
- Theo Bessel — Flight Software Maintainer

For technical questions, contact the TOLOSAT Flight Software team at tolosat.3u@gmail.com.
