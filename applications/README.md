# TOLOSAT Flight Software Applications

The application layer contains the TOLOSAT mission tasks and subsystem logic. It currently includes AOCS, gravimetry, Iridium, power, thermal, and TM/TC components.

Each component is stored under `components/<name>/`:

- `src/` contains its implementation;
- `inc/` contains headers private to the application layer.

System objects such as tasks, buffers, mutexes, file-system entries, timers, and callbacks are described in `system.json`. The pre-build step generates the corresponding C sources and headers under `build/applications/conf/`; generated files must not be edited manually.

Application options belong in `gen/Kconfig.options`. The project configuration sources this fragment and generates a scoped `build/applications/conf/autoconf.h`. The fragment is currently empty, but the standalone configuration workflow is already available:

```bash
cd applications
make default_defconfig
make menuconfig
```

From the repository root:

```bash
make applications
make applications-verif
make applications-clean
```

The application archive is written to `build/libs/libapplications.a`.
