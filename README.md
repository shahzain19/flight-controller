# Flight Computer

A modular, safety-critical flight computer system for unmanned aerial vehicles.

## Overview

This project implements a real-time flight computer software stack designed for reliability, testability, and maintainability. The system follows a strict layered architecture with clear dependency rules to ensure safety and correctness.

## Architecture

The software is organized into strictly layered subsystems:

```
Application (Control, Navigation, Telemetry)
        ↓
    Hardware Abstraction
        ↓
      System Services
        ↓
        Math
        ↓
       Core
```

**Key Principles:**
- **Dependency flows downward only** — core never depends on navigation, math never depends on hardware
- **Simulation is first-class** — the entire system runs without physical hardware
- **Documentation is mandatory** — every subsystem has design docs, requirements, and tests
- **Interfaces are separated from implementations** — `include/` for public APIs, `src/` for internals

## Directory Structure

```
FlightComputer/
├── include/fc/          # Public interfaces (headers)
│   ├── core/            # Fundamental types, status codes, utilities
│   ├── math/            # Vectors, matrices, geometry, filters
│   ├── system/          # Scheduler, timing, watchdog, state machine
│   ├── hardware/        # Sensor, actuator, communication, power abstractions
│   ├── navigation/      # IMU, GPS, sensor fusion, estimation
│   ├── control/         # PID controllers, autopilot, guidance
│   ├── telemetry/       # Logging, packets, diagnostics
│   └── simulation/      # Aircraft models, sensor sim, environment
│
├── src/                 # Implementations (private)
├── tests/               # Unit, integration, simulation, regression tests
├── config/              # Runtime configurations and profiles
├── tools/               # Static analysis, profiling, telemetry viewer
├── docs/                # Architecture, requirements, standards, verification
└── artifacts/           # Coverage reports, benchmarks
```

## Build

```bash
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build

# Test
ctest --test-dir build
```

## Configuration Profiles

| Profile | Purpose |
|---------|---------|
| `simulation` | Desktop testing without hardware |
| `development` | On-target debugging |
| `production` | Flight-ready optimized build |

## Testing

- **Unit tests** — individual module correctness
- **Integration tests** — subsystem interaction verification
- **Simulation scenarios** — full mission replay and validation
- **Regression tests** — prevent known bug recurrence

## Safety

This system is designed for safety-critical operation. All code must comply with the coding standard and safety rules defined in `docs/standards/`.

## Documentation

- [System Overview](docs/architecture/system_overview.md)
- [Software Architecture](docs/architecture/software_architecture.md)
- [Data Flow](docs/architecture/data_flow.md)
- [System Requirements](docs/requirements/system_requirements.md)
- [Coding Standard](docs/standards/coding_standard.md)
- [Safety Rules](docs/standards/safety_rules.md)
- [Test Plan](docs/verification/test_plan.md)

## License

See [LICENSE](LICENSE).
