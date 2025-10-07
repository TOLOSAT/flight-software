# TAPAS Generation

This gen folder contains all the makefiles as well as some files allowing to build and launch the flight software. Indeed, the project relies on a set of makefiles to create it. At the root of the repository, we have a Makefile that includes all the makefiles. We kept a simple Makefile at the root so that it could act as an interface between the user and the other makefiles. In the "gen" directory, we find 4 types of files: the path Makefile, the settings Makefiles, the debug Makefiles, and finally the generation files.

## Paths Makefile

The path.mk Makefile contains all the paths useful to the flight software. This Makefile only stores the paths in variables so that other Makefiles can more easily access them. For example, instead of using "./build", we define "BUILD_DIR". This is very useful when the paths are very long and allows us to store the path only once, which makes it easier to modify the software.

## Settings Makefiles

The settings type makefiles are used to store environment variables other than paths. This includes compilation rules. The settings.mk makefile is responsible for verifying the environment, retrieving the compiler, and calling the other makefiles. The board_setting.mk makefile allows defining variables specific to each board using conditional statements. The cc_settings.mk makefile groups all the compilation flags, including CFLAGS, LDFLAGS. The first is used for compilation (indicating the processor type, displaying warnings, standards, etc.), the second for linkage (which libraries to take), and the last contains optimization and debugging rules.

## Build Makefiles

The build type makefiles are at the heart of the project. They are responsible for retrieving the source files and compiling them with the appropriate rules. There is a generic build file that takes care of the linking phase and calls the other build makefiles. Then, there are makefiles for the core, OS, middlewares, and HALs. Each of these makefiles is quite similar: 1. It retrieves the necessary flags (CFLAGS, INCFLAGS) 2. It lists the source files 3. It compiles the source files into object files. There may be a fourth step to generate a library (useful for the HAL and middlewares).

## Generation Files

The last type of files in the gen folder are generation files. These files allow the flight software to be configured mainly using header files with define statements. We also have linker script files which are currently located here but will likely be moved to the BSP (Board Support Package) folder.