# TAPAS Tools

This directory contains scripts used by the build and development workflows.

| Path | Purpose |
|------|---------|
| `system-parser.py` | Generate application configuration sources from `applications/system.json`. |
| `crc32-gen.py` | Append the firmware CRC during final image generation. |
| `format-code.sh` | Apply clang-format to project-owned C and header files. |
| `update-doc.sh` | Generate Doxygen output under `build/doxygen/`. |
| `qebab.py` | Interactive hexadecimal TCP terminal for emulator communication. |
| `time-converter.py` | Convert TAPAS CUC/TAI timestamps and generate a time-setting telecommand. |
| `svd/` | CMSIS-SVD descriptions used by debugging tools. |

Scripts invoked by Make should normally be run through their corresponding target (`make format`, `make doc`, or a firmware build). Standalone utilities provide their own command-line help where applicable.
