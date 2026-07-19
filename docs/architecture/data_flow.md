# Data Flow

## Overview

Data flows through the flight computer in a well-defined pipeline: raw sensor measurements are acquired, processed into state estimates, used to compute control commands, and sent to actuators. Telemetry data is produced as a side effect for ground station monitoring.

## Primary Control Loop

The main data path runs at the control loop rate (400 Hz):

```
┌─────────┐    ┌──────────┐    ┌────────────┐    ┌─────────┐    ┌──────────┐
│ Sensors  │───▶│ IMU Pre- │───▶│  Sensor    │───▶│ Flight  │───▶│ Actuator │
│ (raw)    │    │ integ.   │    │  Fusion    │    │ State   │    │ Commands │
└─────────┘    └──────────┘    └────────────┘    └────┬────┘    └──────────┘
                                                       │
                                                       ▼
                                                  ┌─────────┐
                                                  │ Control │
                                                  │  (PID)  │
                                                  └─────────┘
```

### Stage 1: Sensor Acquisition

**Input:** Raw hardware signals (accelerometer, gyroscope, magnetometer, barometer, GPS)

**Processing:**
- Raw readings are pulled from sensor drivers via the hardware abstraction layer
- Values are calibrated (bias removal, scale factor correction, temperature compensation)
- Timestamps are assigned based on the system timer

**Output:** Calibrated sensor measurements with timestamps

**Data structures:**
- `ImuSample { Vec3 accel; Vec3 gyro; float temperature; uint64_t timestamp_us; }`
- `GpsFix { Vec3 position; Vec3 velocity; float accuracy; uint64_t timestamp_us; }`
- `BaroReading { float altitude; float pressure; uint64_t timestamp_us; }`

### Stage 2: IMU Preintegration

**Input:** Calibrated IMU samples

**Processing:**
- Gyroscope and accelerometer readings are integrated between navigation update frames
- Incremental rotation and translation are computed and stored
- Bias correction is applied using the latest estimates

**Output:** Preintegrated IMU measurement for the current navigation frame

**Data structures:**
- `PreintegratedImu { Mat3 delta_rotation; Vec3 delta_velocity; Vec3 delta_position; Mat3 covariance; }`

### Stage 3: Sensor Fusion

**Input:** Preintegrated IMU, GPS fixes, barometer, magnetometer

**Processing:**
- An Extended Kalman Filter (EKF) fuses all sensor sources
- Prediction step uses IMU preintegration
- Update steps use GPS (position, velocity), barometer (altitude), magnetometer (heading)
- Sensor health is monitored; faulty sensors are excluded from the update

**Output:** Optimal state estimate with covariance

**Data structures:**
- `FlightState { Vec3 position_ned; Vec3 velocity_ned; Quat attitude; Vec3 accel_body; Vec3 gyro_bias; Vec3 accel_bias; }`

### Stage 4: Control Computation

**Input:** Current flight state, desired state (from autopilot or pilot), mission reference

**Processing:**
- PID controllers compute attitude commands (roll, pitch, yaw rate)
- Outer loops compute velocity and position commands
- Output is mixed into individual motor/servo commands
- Limits and saturation checks are applied

**Output:** Actuator command set

**Data structures:**
- `ControlOutput { float motor_commands[4]; float servo_angles[2]; }`

### Stage 5: Actuator Output

**Input:** Actuator command set

**Processing:**
- Commands are scaled to hardware range (PWM values, protocol-specific formats)
- Safety limits are enforced (minimum/maximum thrust, rate limits)
- Commands are sent to actuators via the hardware abstraction layer

**Output:** Physical actuation (motor speeds, servo positions)

## Navigation Update Loop

Runs at 100 Hz, interleaved with the control loop:

```
┌──────────┐    ┌────────────┐    ┌─────────────┐
│ GPS Fix  │───▶│  Sensor    │───▶│   State     │
│ Barometer│    │  Fusion    │    │  Estimate   │
│ Magnetom.│    │   (EKF)    │    │  (shared)   │
└──────────┘    └────────────┘    └─────────────┘
```

The state estimate is written to a shared structure that the control loop reads at 400 Hz. Between navigation updates, the control loop uses the latest available estimate.

## Telemetry Data Flow

Runs at 10 Hz:

```
┌─────────────┐    ┌──────────────┐    ┌──────────┐
│ Flight State │───▶│   Packet     │───▶│  Comm    │
│ Diagnostics  │    │  Encoder     │    │  Link    │
│ Mission Data │    │              │    │ (radio)  │
└─────────────┘    └──────────────┘    └──────────┘
```

Telemetry data is assembled from multiple sources and encoded into compact packets for transmission to the ground station. Incoming commands from the ground station are decoded and routed to the appropriate subsystem.

## Simulation Data Flow

In simulation mode, the data flow is identical except that hardware drivers are replaced:

```
┌──────────┐    ┌──────────┐    ┌──────────┐
│ Aircraft  │───▶│  Sensor  │───▶│  Normal  │
│  Model    │    │   Sim    │    │  Pipeline│
└──────────┘    └──────────┘    └──────────┘
       ▲                               │
       │          ┌──────────┐         │
       └──────────│ Actuator │◀────────┘
                  │   Sim    │
                  └──────────┘
```

The simulation loop:
1. Aircraft model receives actuator commands and computes new state
2. Sensor simulator generates realistic readings from the aircraft state
3. Normal pipeline processes the simulated data
4. Control outputs feed back into the aircraft model

This closed-loop simulation enables full system validation without hardware.

## Data Ownership Rules

| Data | Writer | Readers | Access Pattern |
|------|--------|---------|----------------|
| Raw sensor data | Sensor drivers | Preintegration, Fusion | Single-writer |
| Preintegrated IMU | Preintegration | Fusion | Single-writer |
| Flight state | Fusion | Control, Telemetry, Logging | Single-writer, multi-reader |
| Control output | Control | Actuator drivers, Telemetry | Single-writer |
| Mission commands | Guidance | Control | Single-writer |
| Telemetry packets | Telemetry encoder | Comm link | Single-writer |

All shared data uses lock-free single-writer patterns. The control loop always reads the latest complete state snapshot; navigation updates are atomic.
