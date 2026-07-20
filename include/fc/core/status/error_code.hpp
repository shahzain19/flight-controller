/// @file error_code.hpp
/// @brief Error code definitions for the flight computer.
///
/// Every error condition is represented as an enumerated error code.
/// Error codes are grouped by subsystem for clarity. The kOk code
/// represents success and must be the zero value.
///
/// Design:
///   - Error codes are grouped by subsystem (core, math, hardware, etc.)
///   - kOk is always 0 for fast success checks
///   - Values are explicitly assigned for ABI stability
///   - The enum is backed by a 16-bit integer for compact storage
///
/// Usage:
///   Status s = some_operation();
///   if (s.code() == ErrorCode::kOk) { ... }
///   if (s.code() == ErrorCode::kSensorTimeout) { ... }
///
/// @defgroup core_error_code Error Codes
/// @{

#pragma once

#include <cstdint>

namespace fc {

/// @brief Error code enumeration for the flight computer.
///
/// Backed by std::uint16_t to ensure fixed size across platforms.
/// Values are explicitly assigned for ABI stability.
enum class ErrorCode : std::uint16_t {
    // ========================================================================
    // Success
    // ========================================================================

    /// @brief Operation completed successfully.
    kOk = 0,

    // ========================================================================
    // Core Errors (0x0100 - 0x01FF)
    // ========================================================================

    /// @brief An invalid argument was passed to a function.
    kInvalidArgument = 0x0100,

    /// @brief An index is out of bounds.
    kOutOfBounds = 0x0101,

    /// @brief A null pointer was dereferenced.
    kNullPointer = 0x0102,

    /// @brief A buffer is too small to hold the result.
    kBufferTooSmall = 0x0103,

    /// @brief An operation is not supported on this platform.
    kNotSupported = 0x0104,

    /// @brief An operation exceeded a timeout.
    kTimeout = 0x0105,

    /// @brief A resource is already in use.
    kAlreadyInUse = 0x0106,

    /// @brief A resource was not found.
    kNotFound = 0x0107,

    /// @brief An internal invariant was violated.
    kInternalError = 0x0108,

    /// @brief The system is not initialized.
    kNotInitialized = 0x0109,

    /// @brief The operation is not permitted in the current state.
    kIllegalState = 0x010A,

    // ========================================================================
    // Math Errors (0x0200 - 0x02FF)
    // ========================================================================

    /// @brief A division by zero was attempted.
    kDivisionByZero = 0x0200,

    /// @brief A numerical overflow occurred.
    kOverflow = 0x0201,

    /// @brief A numerical underflow occurred.
    kUnderflow = 0x0202,

    /// @brief A NaN value was encountered where a finite value is required.
    kNaN = 0x0203,

    /// @brief An infinity value was encountered where a finite value is required.
    kInfinity = 0x0204,

    /// @brief A matrix operation failed (e.g., singular matrix).
    kSingularMatrix = 0x0205,

    /// @brief A quaternion is not normalized.
    kQuaternionNotNormalized = 0x0206,

    /// @brief A numerical result is out of acceptable bounds.
    kNumericalBounds = 0x0207,

    // ========================================================================
    // Hardware Errors (0x0300 - 0x03FF)
    // ========================================================================

    /// @brief A sensor returned invalid data.
    kSensorInvalidData = 0x0300,

    /// @brief A sensor is not responding.
    kSensorTimeout = 0x0301,

    /// @brief A sensor is not calibrated.
    kSensorNotCalibrated = 0x0302,

    /// @brief An actuator command failed.
    kActuatorFailure = 0x0303,

    /// @brief A communication link is down.
    kCommLinkDown = 0x0304,

    /// @brief A communication error occurred.
    kCommError = 0x0305,

    /// @brief A power system fault was detected.
    kPowerFault = 0x0306,

    /// @brief A hardware watchdog timeout occurred.
    kWatchdogTimeout = 0x0307,

    /// @brief A CRC or checksum verification failed.
    kChecksumError = 0x0308,

    /// @brief A hardware register read/write failed.
    kHardwareFault = 0x0309,

    // ========================================================================
    // Navigation Errors (0x0400 - 0x04FF)
    // ========================================================================

    /// @brief GPS fix is not available.
    kGPSNoFix = 0x0400,

    /// @brief GPS accuracy is below threshold.
    kGPSAccuracy = 0x0401,

    /// @brief The EKF divergence was detected.
    kEKFDivergence = 0x0402,

    /// @brief Sensor fusion failed to converge.
    kFusionFailure = 0x0403,

    /// @brief The state estimate is invalid.
    kStateEstimateInvalid = 0x0404,

    /// @brief The reference frame is invalid.
    kInvalidFrame = 0x0405,

    // ========================================================================
    // Control Errors (0x0500 - 0x05FF)
    // ========================================================================

    /// @brief A PID controller gain is invalid.
    kInvalidGain = 0x0500,

    /// @brief The control output is saturated.
    kControlSaturated = 0x0501,

    /// @brief The autopilot mode transition is invalid.
    kInvalidModeTransition = 0x0502,

    /// @brief The guidance law failed to converge.
    kGuidanceFailure = 0x0503,

    /// @brief The mission waypoint is invalid.
    kInvalidWaypoint = 0x0504,

    // ========================================================================
    // System Errors (0x0600 - 0x06FF)
    // ========================================================================

    /// @brief A task deadline was missed.
    kDeadlineMissed = 0x0600,

    /// @brief The scheduler is in an invalid state.
    kSchedulerFault = 0x0601,

    /// @brief A configuration parameter is invalid.
    kInvalidConfig = 0x0602,

    /// @brief The system is in a degraded mode.
    kDegradedMode = 0x0603,

    /// @brief A safety constraint was violated.
    kSafetyViolation = 0x0604,

    /// @brief The flight computer is disarmed.
    kDisarmed = 0x0605,

    /// @brief The emergency stop was triggered.
    kEmergencyStop = 0x0606,

    // ========================================================================
    // Telemetry Errors (0x0700 - 0x07FF)
    // ========================================================================

    /// @brief A telemetry packet failed to encode.
    kPacketEncodeError = 0x0700,

    /// @brief A telemetry packet failed to decode.
    kPacketDecodeError = 0x0701,

    /// @brief A logging buffer overflowed.
    kLogBufferOverflow = 0x0702,

    /// @brief The telemetry link is not connected.
    kTelemetryDisconnected = 0x0703,
};

/// @brief Returns a human-readable string for an error code.
///
/// @param code The error code to convert.
/// @return A pointer to a static string literal. Never returns nullptr.
///
/// @note The returned pointer is valid for the lifetime of the program.
[[nodiscard]] constexpr const char* error_code_to_string(ErrorCode code) noexcept {
    switch (code) {
        case ErrorCode::kOk: return "Ok";

        // Core
        case ErrorCode::kInvalidArgument: return "InvalidArgument";
        case ErrorCode::kOutOfBounds: return "OutOfBounds";
        case ErrorCode::kNullPointer: return "NullPointer";
        case ErrorCode::kBufferTooSmall: return "BufferTooSmall";
        case ErrorCode::kNotSupported: return "NotSupported";
        case ErrorCode::kTimeout: return "Timeout";
        case ErrorCode::kAlreadyInUse: return "AlreadyInUse";
        case ErrorCode::kNotFound: return "NotFound";
        case ErrorCode::kInternalError: return "InternalError";
        case ErrorCode::kNotInitialized: return "NotInitialized";
        case ErrorCode::kIllegalState: return "IllegalState";

        // Math
        case ErrorCode::kDivisionByZero: return "DivisionByZero";
        case ErrorCode::kOverflow: return "Overflow";
        case ErrorCode::kUnderflow: return "Underflow";
        case ErrorCode::kNaN: return "NaN";
        case ErrorCode::kInfinity: return "Infinity";
        case ErrorCode::kSingularMatrix: return "SingularMatrix";
        case ErrorCode::kQuaternionNotNormalized: return "QuaternionNotNormalized";
        case ErrorCode::kNumericalBounds: return "NumericalBounds";

        // Hardware
        case ErrorCode::kSensorInvalidData: return "SensorInvalidData";
        case ErrorCode::kSensorTimeout: return "SensorTimeout";
        case ErrorCode::kSensorNotCalibrated: return "SensorNotCalibrated";
        case ErrorCode::kActuatorFailure: return "ActuatorFailure";
        case ErrorCode::kCommLinkDown: return "CommLinkDown";
        case ErrorCode::kCommError: return "CommError";
        case ErrorCode::kPowerFault: return "PowerFault";
        case ErrorCode::kWatchdogTimeout: return "WatchdogTimeout";
        case ErrorCode::kChecksumError: return "ChecksumError";
        case ErrorCode::kHardwareFault: return "HardwareFault";

        // Navigation
        case ErrorCode::kGPSNoFix: return "GPSNoFix";
        case ErrorCode::kGPSAccuracy: return "GPSAccuracy";
        case ErrorCode::kEKFDivergence: return "EKFDivergence";
        case ErrorCode::kFusionFailure: return "FusionFailure";
        case ErrorCode::kStateEstimateInvalid: return "StateEstimateInvalid";
        case ErrorCode::kInvalidFrame: return "InvalidFrame";

        // Control
        case ErrorCode::kInvalidGain: return "InvalidGain";
        case ErrorCode::kControlSaturated: return "ControlSaturated";
        case ErrorCode::kInvalidModeTransition: return "InvalidModeTransition";
        case ErrorCode::kGuidanceFailure: return "GuidanceFailure";
        case ErrorCode::kInvalidWaypoint: return "InvalidWaypoint";

        // System
        case ErrorCode::kDeadlineMissed: return "DeadlineMissed";
        case ErrorCode::kSchedulerFault: return "SchedulerFault";
        case ErrorCode::kInvalidConfig: return "InvalidConfig";
        case ErrorCode::kDegradedMode: return "DegradedMode";
        case ErrorCode::kSafetyViolation: return "SafetyViolation";
        case ErrorCode::kDisarmed: return "Disarmed";
        case ErrorCode::kEmergencyStop: return "EmergencyStop";

        // Telemetry
        case ErrorCode::kPacketEncodeError: return "PacketEncodeError";
        case ErrorCode::kPacketDecodeError: return "PacketDecodeError";
        case ErrorCode::kLogBufferOverflow: return "LogBufferOverflow";
        case ErrorCode::kTelemetryDisconnected: return "TelemetryDisconnected";

        default: return "Unknown";
    }
}

/// @brief Returns true if the error code represents success.
[[nodiscard]] constexpr bool is_ok(ErrorCode code) noexcept {
    return code == ErrorCode::kOk;
}

/// @brief Returns true if the error code represents an error.
[[nodiscard]] constexpr bool is_error(ErrorCode code) noexcept {
    return code != ErrorCode::kOk;
}

} // namespace fc

/// @}
