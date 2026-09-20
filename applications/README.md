# TOLOSAT Flight Software Applications

The application layer contains the TOLOSAT mission tasks and subsystem logic. It currently includes AOCS, gravimetry, Iridium, power, thermal, and TM/TC components.

Each component is stored under `components/<name>/`:

- `src/` contains its implementation;
- `inc/` contains headers private to the application layer.

System objects such as tasks, buffers, mutexes, file-system entries, timers, and callbacks are described in `system.json`. The pre-build step generates the corresponding C sources and headers under `build/applications/conf/`; generated files must not be edited manually.

From the repository root:

```bash
make applications
make applications-verif
make applications-clean
```

The application archive is written to `build/libs/libapplications.a`.
