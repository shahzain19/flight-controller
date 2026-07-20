/// @file constants.hpp
/// @brief Mathematical, physical, and system constants for the flight computer.
///
/// All constants are defined as constexpr to enable compile-time evaluation.
/// They are organized into namespaces for clarity.
///
/// Usage:
///   f64 angle = fc::constants::kPi;
///   f64 altitude = fc::constants::physics::kStandardGravity;
///
/// Compliance:
///   - All constants are constexpr
///   - No dynamic allocation
///   - Units are documented in comments
///
/// @defgroup core_constants Constants
/// @{

#pragma once

#include "../types/types.hpp"

namespace fc::constants {

// ============================================================================
// Mathematical Constants
// ============================================================================

/// @namespace math
/// @brief Mathematical constants.
namespace math {

/// @brief Pi (π). Ratio of circumference to diameter.
inline constexpr f64 kPi = 3.14159265358979323846;

/// @brief Half pi (π/2).
inline constexpr f64 kHalfPi = kPi / 2.0;

/// @brief Quarter pi (π/4).
inline constexpr f64 kQuarterPi = kPi / 4.0;

/// @brief Two pi (2π). Full circle in radians.
inline constexpr f64 kTwoPi = 2.0 * kPi;

/// @brief Inverse pi (1/π).
inline constexpr f64 kInvPi = 1.0 / kPi;

/// @brief Square root of 2.
inline constexpr f64 kSqrt2 = 1.41421356237309504880;

/// @brief Square root of 3.
inline constexpr f64 kSqrt3 = 1.73205080756887729353;

/// @brief Inverse square root of 2.
inline constexpr f64 kInvSqrt2 = 1.0 / kSqrt2;

/// @brief Euler's number (e). Base of natural logarithm.
inline constexpr f64 kE = 2.71828182845904523536;

/// @brief Natural logarithm of 2.
inline constexpr f64 kLn2 = 0.69314718055994530942;

/// @brief Natural logarithm of 10.
inline constexpr f64 kLn10 = 2.30258509299404568402;

/// @brief Degrees to radians conversion factor.
inline constexpr f64 kDegreesToRadians = kPi / 180.0;

/// @brief Radians to degrees conversion factor.
inline constexpr f64 kRadiansToDegrees = 180.0 / kPi;

/// @brief Arcminutes to radians conversion factor.
inline constexpr f64 kArcminutesToRadians = kDegreesToRadians / 60.0;

/// @brief Arcseconds to radians conversion factor.
inline constexpr f64 kArcsecondsToRadians = kArcminutesToRadians / 60.0;

/// @brief Milli-g to m/s^2 conversion factor.
inline constexpr f64 kMilliGToMS2 = 0.001 * 9.80665;

} // namespace math

// ============================================================================
// Physical Constants
// ============================================================================

/// @namespace physics
/// @brief Physical constants (SI units).
namespace physics {

/// @brief Standard acceleration due to gravity (m/s^2).
/// @note This is the nominal value; actual gravity varies by location.
inline constexpr f64 kStandardGravity = 9.80665;

/// @brief Speed of light in vacuum (m/s).
inline constexpr f64 kSpeedOfLight = 299792458.0;

/// @brief Standard atmospheric pressure (Pa).
/// @note At sea level, 15°C, 101325 Pa.
inline constexpr f64 kStandardPressure = 101325.0;

/// @brief Standard air density at sea level (kg/m^3).
/// @note At 15°C, 101325 Pa.
inline constexpr f64 kStandardAirDensity = 1.225;

/// @brief Standard temperature at sea level (K).
/// @note 15°C = 288.15 K.
inline constexpr f64 kStandardTemperature = 288.15;

/// @brief Universal gas constant (J/(mol·K)).
inline constexpr f64 kGasConstant = 8.314462618;

/// @brief Molar mass of dry air (kg/mol).
inline constexpr f64 kMolarMassDryAir = 0.0289644;

/// @brief Boltzmann constant (J/K).
inline constexpr f64 kBoltzmann = 1.380649e-23;

/// @brief Earth's mean radius (m).
/// @note WGS-84 ellipsoid mean radius.
inline constexpr f64 kEarthRadius = 6371000.0;

/// @brief Earth's gravitational parameter (m^3/s^2).
/// @note GM for Earth.
inline constexpr f64 kEarthGM = 3.986004418e14;

/// @brief Earth's rotation rate (rad/s).
/// @note Sidereal day rotation rate.
inline constexpr f64 kEarthRotationRate = 7.2921159e-5;

/// @brief Conversion from feet to meters.
inline constexpr f64 kFeetToMeters = 0.3048;

/// @brief Conversion from meters to feet.
inline constexpr f64 kMetersToFeet = 1.0 / kFeetToMeters;

/// @brief Conversion from knots to m/s.
inline constexpr f64 kKnotsToMS = 0.514444;

/// @brief Conversion from m/s to knots.
inline constexpr f64 kMSToKnots = 1.0 / kKnotsToMS;

/// @brief Conversion from miles to meters.
inline constexpr f64 kMilesToMeters = 1609.344;

/// @brief Conversion from meters to miles.
inline constexpr f64 kMetersToMiles = 1.0 / kMilesToMeters;

} // namespace physics

// ============================================================================
// System Constants
// ============================================================================

/// @namespace system
/// @brief System-level constants for the flight computer.
namespace system {

/// @brief Control loop frequency (Hz).
inline constexpr u32 kControlLoopHz = 400;

/// @brief Navigation update frequency (Hz).
inline constexpr u32 kNavigationHz = 100;

/// @brief Telemetry downlink frequency (Hz).
inline constexpr u32 kTelemetryHz = 10;

/// @brief Watchdog timeout (microseconds).
/// @note If the main loop does not service the watchdog within this time,
///       the system resets.
inline constexpr u64 kWatchdogTimeoutUs = 1000;

/// @brief Maximum number of motors supported.
inline constexpr u32 kMaxMotors = 8;

/// @brief Maximum number of servos supported.
inline constexpr u32 kMaxServos = 4;

/// @brief Maximum number of sensors supported.
inline constexpr u32 kMaxSensors = 16;

/// @brief Maximum telemetry packet size (bytes).
inline constexpr u32 kMaxPacketSize = 1024;

/// @brief Maximum log message length (bytes).
inline constexpr u32 kMaxLogMessageLength = 256;

/// @brief Maximum number of tasks in the scheduler.
inline constexpr u32 kMaxSchedulerTasks = 32;

/// @brief Maximum mission waypoints.
inline constexpr u32 kMaxWaypoints = 256;

/// @brief Stack size for the main thread (bytes).
/// @note This is a default; actual size depends on the platform.
inline constexpr u32 kMainStackSize = 65536;

/// @brief Invalid sensor ID sentinel value.
inline constexpr u32 kInvalidSensorId = 0xFFFFFFFF;

/// @brief Invalid waypoint ID sentinel value.
inline constexpr u32 kInvalidWaypointId = 0xFFFFFFFF;

} // namespace system

} // namespace fc::constants

/// @}
