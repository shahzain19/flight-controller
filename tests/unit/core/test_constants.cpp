/// @file test_constants.cpp
/// @brief Unit tests for fc::constants.

#include <fc/core/utilities/constants.hpp>

#include <cassert>
#include <cmath>
#include <cstdio>

// ============================================================================
// Mathematical Constants Tests
// ============================================================================

void test_pi() {
    assert(fc::constants::math::kPi > 3.14159);
    assert(fc::constants::math::kPi < 3.14160);
    assert(std::abs(fc::constants::math::kPi - 3.14159265358979323846) < 1e-15);

    std::printf("PASS: kPi\n");
}

void test_two_pi() {
    assert(std::abs(fc::constants::math::kTwoPi - 2.0 * fc::constants::math::kPi) < 1e-15);

    std::printf("PASS: kTwoPi\n");
}

void test_half_pi() {
    assert(std::abs(fc::constants::math::kHalfPi - fc::constants::math::kPi / 2.0) < 1e-15);

    std::printf("PASS: kHalfPi\n");
}

void test_sqrt2() {
    assert(std::abs(fc::constants::math::kSqrt2 - std::sqrt(2.0)) < 1e-15);

    std::printf("PASS: kSqrt2\n");
}

void test_euler() {
    assert(std::abs(fc::constants::math::kE - 2.718281828459045) < 1e-15);

    std::printf("PASS: kE\n");
}

void test_degrees_radians_conversion() {
    fc::f64 degrees = 180.0;
    fc::f64 radians = degrees * fc::constants::math::kDegreesToRadians;
    assert(std::abs(radians - fc::constants::math::kPi) < 1e-10);

    fc::f64 back = radians * fc::constants::math::kRadiansToDegrees;
    assert(std::abs(back - 180.0) < 1e-10);

    std::printf("PASS: degrees/radians conversion\n");
}

// ============================================================================
// Physical Constants Tests
// ============================================================================

void test_standard_gravity() {
    assert(std::abs(fc::constants::physics::kStandardGravity - 9.80665) < 1e-5);

    std::printf("PASS: kStandardGravity\n");
}

void test_speed_of_light() {
    assert(std::abs(fc::constants::physics::kSpeedOfLight - 299792458.0) < 1.0);

    std::printf("PASS: kSpeedOfLight\n");
}

void test_earth_radius() {
    assert(fc::constants::physics::kEarthRadius > 6370000.0);
    assert(fc::constants::physics::kEarthRadius < 6372000.0);

    std::printf("PASS: kEarthRadius\n");
}

void test_unit_conversions() {
    // Feet to meters
    fc::f64 feet = 1000.0;
    fc::f64 meters = feet * fc::constants::physics::kFeetToMeters;
    assert(std::abs(meters - 304.8) < 0.01);

    // Knots to m/s
    fc::f64 knots = 100.0;
    fc::f64 ms = knots * fc::constants::physics::kKnotsToMS;
    assert(ms > 50.0 && ms < 52.0);

    std::printf("PASS: unit conversions\n");
}

// ============================================================================
// System Constants Tests
// ============================================================================

void test_control_loop_frequency() {
    assert(fc::constants::system::kControlLoopHz == 400);

    std::printf("PASS: kControlLoopHz\n");
}

void test_navigation_frequency() {
    assert(fc::constants::system::kNavigationHz == 100);

    std::printf("PASS: kNavigationHz\n");
}

void test_telemetry_frequency() {
    assert(fc::constants::system::kTelemetryHz == 10);

    std::printf("PASS: kTelemetryHz\n");
}

void test_max_motors() {
    assert(fc::constants::system::kMaxMotors == 8);

    std::printf("PASS: kMaxMotors\n");
}

void test_watchdog_timeout() {
    assert(fc::constants::system::kWatchdogTimeoutUs == 1000);

    std::printf("PASS: kWatchdogTimeoutUs\n");
}

// ============================================================================
// Constexpr Tests
// ============================================================================

void test_constexpr_usage() {
    // Verify constants can be used in constexpr contexts
    constexpr fc::f64 pi = fc::constants::math::kPi;
    constexpr fc::f64 gravity = fc::constants::physics::kStandardGravity;
    constexpr fc::u32 hz = fc::constants::system::kControlLoopHz;

    assert(pi > 3.0);
    assert(gravity > 9.0);
    assert(hz == 400);

    std::printf("PASS: constexpr usage\n");
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::printf("Running constants tests...\n");

    test_pi();
    test_two_pi();
    test_half_pi();
    test_sqrt2();
    test_euler();
    test_degrees_radians_conversion();
    test_standard_gravity();
    test_speed_of_light();
    test_earth_radius();
    test_unit_conversions();
    test_control_loop_frequency();
    test_navigation_frequency();
    test_telemetry_frequency();
    test_max_motors();
    test_watchdog_timeout();
    test_constexpr_usage();

    std::printf("All constants tests passed.\n");
    return 0;
}
