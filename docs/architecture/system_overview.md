# System Overview

## Purpose

The Flight Computer is a real-time, safety-critical embedded system responsible for autonomous or semi-autonomous operation of an unmanned aerial vehicle (UAV). It handles sensor acquisition, state estimation, navigation, control, communication, and system health monitoring.

## System Context

```
┌─────────────────────────────────────────────────────┐
│                  Flight Computer                     │
│                                                      │
│  ┌──────────┐  ┌──────────┐  ┌──────────────────┐  │
│  │ Navigation│  │ Control  │  │    Telemetry     │  │
│  └─────┬────┘  └────┬─────┘  └────────┬─────────┘  │
│        │             │                  │             │
│  ┌─────▼─────────────▼──────────────────▼─────────┐  │
│  │              System Services                    │  │
│  │        (Scheduler, Timing, Watchdog)            │  │
│  └─────────────────────┬──────────────────────────┘  │
│                        │                             │
│  ┌─────────────────────▼──────────────────────────┐  │
│  │           Hardware Abstraction                  │  │
│  │    (Sensors, Actuators, Comm, Power)            │  │
│  └────────────────────────────────────────────────┘  │
└──────────────────────────┬──────────────────────────┘
                           │
        ┌──────────────────┼──────────────────┐
        ▼                  ▼                  ▼
   ┌─────────┐      ┌──────────┐      ┌──────────┐
   │ Sensors  │      │Actuators │      │   Comm   │
   │ IMU,GPS  │      │ Motors,  │      │  Radio,  │
   │ Baro, etc│      │ Servos   │      │  Serial  │
   └─────────┘      └──────────┘      └──────────┘
```

## Subsystem Summary

| Subsystem | Responsibility | Key Interfaces |
|-----------|---------------|----------------|
| **Core** | Fundamental types, error codes, utility functions | `types.hpp`, `status.hpp` |
| **Math** | Linear algebra, geometry, signal filters | `Vector3`, `Matrix3x3`, `KalmanFilter` |
| **System** | Task scheduling, time management, watchdog, state machine | `Scheduler`, `Timer`, `StateMachine` |
| **Hardware** | Abstraction over physical devices | `SensorDriver`, `ActuatorDriver`, `CommLink` |
| **Navigation** | State estimation, position/velocity/attitude determination | `IMUPreintegrator`, `GPSSource`, `SensorFusion`, `StateEstimator` |
| **Control** | Stabilization, path following, mission execution | `PIDController`, `Autopilot`, `GuidanceLaw` |
| **Telemetry** | Data logging, ground station communication, health reporting | `Logger`, `PacketEncoder`, `Diagnostics` |
| **Simulation** | Virtual aircraft, sensors, and environment for offline testing | `AircraftModel`, `SensorSim`, `EnvironmentModel` |

## Operational Modes

| Mode | Description |
|------|-------------|
| **Simulation** | Full system running on desktop, no hardware required |
| **Development** | On-target with debug instrumentation |
| **Production** | Optimized, minimal logging, full safety checks |

## Safety Requirements

- The watchdog must reset the system if the main loop fails to execute within the configured timeout
- The state machine enforces legal mode transitions (e.g., cannot arm without valid sensor health)
- All sensor inputs are validated before use in control loops
- The system must detect and report sensor failures within one control cycle

## Performance Targets

| Metric | Target |
|--------|--------|
| Control loop rate | 400 Hz |
| Navigation update rate | 100 Hz |
| Telemetry downlink rate | 10 Hz |
| End-to-end sensor-to-actuator latency | < 10 ms |
