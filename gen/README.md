# TAPAS Build System

The `gen/` directory contains the top-level build and configuration logic. It is source-controlled; generated build artefacts are written exclusively under `build/`.

## Files

| File or directory | Responsibility |
|-------------------|----------------|
| `Kconfig` | Top-level firmware configuration and board selection. |
| `paths.mk` | Repository and output paths. |
| `settings.mk` | Tool selection, configuration loading, and environment checks. |
| `cc-settings.mk` | Common compiler and linker flags. |
| `pre-build.mk` | Generation of application configuration sources. |
| `externals.mk` | Invocation of the kernel and middleware submodules. |
| `build.mk` | Build orchestration and final firmware link. |
| `configuration.mk` | Kconfig and defconfig targets. |
| `verification.mk` | Static analysis, formatting, and documentation targets. |
| `upload.mk` | Upload, emulator, and debug targets. |
| `help.mk` | Command-line target summary. |
| `MISRA/` | Cppcheck/MISRA configuration. |
| `doxygen/` | Doxygen configuration. |

## Build flow

The build first generates configuration-dependent sources, then builds the kernel, applications, and middleware archives before linking the final ELF. Sources are currently discovered automatically from each component's `src/` directory.

Every archive has a content-stable description under `build/state/*.flags`. It records the effective compiler, compilation flags, include paths, and source list. Objects depend on the corresponding description, so a flag change rebuilds only the affected build domain. The link has an equivalent `build/state/link.flags` file.

Generated configuration files live under:

- `build/applications/conf/` for application configuration;
- `build/kernel/conf/` for kernel and BSP configuration.

Use `make print-<variable>` from the repository root to inspect an effective Make variable without editing a Makefile.
