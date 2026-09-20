# TAPAS Middleware Libraries

Middleware libraries provide reusable services above the kernel API and below mission applications. Each library is maintained as a Git submodule.

| Library | Purpose |
|---------|---------|
| `pus-library` | Packet Utilization Standard packet management and services. |
| `iridium-library` | Iridium 9603 driver and mission-specific PUS service. |
| `thermal-library` | Temperature-sensor driver and thermal PUS service. |

Libraries follow the common layout:

```text
<name>-library/
├── include/                  API exported to applications and other libraries
└── components/
    └── <component>/
        ├── <header>.h        library-internal component API, when required
        ├── inc/              private headers, when required
        └── src/              implementation
```

Build them from the flight-software repository root so the toolchain, kernel headers, dependent libraries, and output paths are provided consistently:

```bash
make pus
make iridium
make thermal
```

The resulting archives are written to `build/libs/`. Use the corresponding `<name>-clean` and `<name>-verif` targets for cleanup and static analysis.
