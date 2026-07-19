# Software Architecture

## Design Philosophy

The flight computer software follows a **strictly layered architecture** with unidirectional dependencies. Each layer only knows about the layer directly below it. This ensures testability, replaceability, and safety.

## Layer Model

```
┌─────────────────────────────────────────────────────┐
│                APPLICATION LAYER                     │
│         (Navigation, Control, Telemetry)             │
├─────────────────────────────────────────────────────┤
│              HARDWARE ABSTRACTION LAYER              │
│       (Sensors, Actuators, Communication, Power)     │
├─────────────────────────────────────────────────────┤
│                  SYSTEM SERVICES LAYER               │
│      (Scheduler, Timing, Watchdog, State Machine)    │
├─────────────────────────────────────────────────────┤
│                      MATH LAYER                      │
│   (Vectors, Matrices, Geometry, Filters)             │
├─────────────────────────────────────────────────────┤
│                      CORE LAYER                      │
│         (Types, Status Codes, Utilities)             │
└─────────────────────────────────────────────────────┘
```

## Dependency Rules

**Direction:** Dependencies flow strictly downward.

```
Application → Hardware Abstraction → System Services → Math → Core
```

**Prohibited dependencies:**
- Core → anything above it (Core depends on nothing)
- Math → Hardware, System, or Application
- System Services → Hardware or Application
- Hardware Abstraction → Application
- Navigation → Control (sibling subsystems do not depend on each other)

**Sibling communication:** When two subsystems at the same layer need to interact, they communicate through a shared data structure or through the layer below. Navigation and Control, for example, share state through a well-defined `FlightState` structure managed by the system services layer.

## Interface Segregation

### include/ Directory

The `include/fc/` directory exposes only the public API of each subsystem. Consumers include headers from this directory. No implementation details are visible.

```
include/fc/navigation/estimation/state_estimator.hpp
    → declares class StateEstimator
    → does NOT include internal filter implementation details
```

### src/ Directory

The `src/` directory contains implementation files. Internal headers, private helper functions, and implementation-specific types live here. They are never included by code outside the subsystem.

```
src/navigation/estimation/
    state_estimator.cpp        → implementation
    internal/                  → private headers (if needed)
```

## Module Template

Every subsystem module follows this structure:

```
module_name/
├── include/
│   └── fc/
│       └── module_name/
│           └── interface.hpp
├── src/
│   └── implementation.cpp
├── tests/
│   └── module_test.cpp
└── README.md
```

## Build System

CMake is the build system. Each subsystem may define its own `CMakeLists.txt` which is included by the top-level build configuration. The build supports three profiles:

| Profile | CMake Define | Characteristics |
|---------|-------------|-----------------|
| Simulation | `-DFC_PROFILE=simulation` | All features enabled, no hardware |
| Development | `-DFC_PROFILE=development` | Debug symbols, extra logging |
| Production | `-DFC_PROFILE=production` | Optimized, minimal overhead |

## Simulation Architecture

The simulation layer provides drop-in replacements for hardware-dependent interfaces:

```
Real Hardware          Simulation
─────────────          ──────────
IMUDriver         →    SimIMU
GPSDriver         →    SimGPS
ActuatorDriver    →    SimActuator
CommLink          →    SimCommLink
```

This allows the entire control and navigation stack to run unmodified on a desktop computer. Simulation scenarios can replay recorded sensor data, inject faults, and validate system behavior.

## Error Handling

Errors are represented as typed status codes defined in `core/status/`. Every function that can fail returns a `Status` or `Result<T>` type. Exceptions are not used in production builds.

```cpp
Status initialize_sensors();
Result<Vec3> read_acceleration();
```

Watchdog and health monitoring subsystems continuously check for degraded operation and trigger appropriate recovery actions.

## Concurrency Model

The system uses a cooperative scheduler with priority-based task dispatch:

| Priority | Task | Rate |
|----------|------|------|
| Critical | Watchdog check | 1000 Hz |
| High | Control loop | 400 Hz |
| Medium | Navigation | 100 Hz |
| Low | Telemetry | 10 Hz |
| Background | Diagnostics, logging | Best effort |

All tasks run in a single thread (deterministic, no race conditions). The scheduler preempts based on priority using timer interrupts.
