/// @file test_error_code.cpp
/// @brief Unit tests for fc::ErrorCode and error_code_to_string.

#include <fc/core/status/error_code.hpp>
#include <fc/core/types/types.hpp>

#include <cassert>
#include <cstdio>
#include <cstring>

// ============================================================================
// Success Code Tests
// ============================================================================

void test_ok_is_zero() {
    static_assert(static_cast<fc::u16>(fc::ErrorCode::kOk) == 0, "kOk must be 0");
    assert(fc::ErrorCode::kOk == fc::ErrorCode::kOk);

    std::printf("PASS: kOk is zero\n");
}

void test_is_ok() {
    assert(fc::is_ok(fc::ErrorCode::kOk));
    assert(!fc::is_ok(fc::ErrorCode::kInvalidArgument));
    assert(!fc::is_ok(fc::ErrorCode::kSensorTimeout));

    std::printf("PASS: is_ok\n");
}

void test_is_error() {
    assert(!fc::is_error(fc::ErrorCode::kOk));
    assert(fc::is_error(fc::ErrorCode::kInvalidArgument));
    assert(fc::is_error(fc::ErrorCode::kSensorTimeout));

    std::printf("PASS: is_error\n");
}

// ============================================================================
// Error Code String Conversion Tests
// ============================================================================

void test_error_code_to_string_success() {
    const char* str = fc::error_code_to_string(fc::ErrorCode::kOk);
    assert(str != nullptr);
    assert(std::strcmp(str, "Ok") == 0);

    std::printf("PASS: error_code_to_string kOk\n");
}

void test_error_code_to_string_core() {
    assert(std::strcmp(fc::error_code_to_string(fc::ErrorCode::kInvalidArgument), "InvalidArgument") == 0);
    assert(std::strcmp(fc::error_code_to_string(fc::ErrorCode::kOutOfBounds), "OutOfBounds") == 0);
    assert(std::strcmp(fc::error_code_to_string(fc::ErrorCode::kNullPointer), "NullPointer") == 0);
    assert(std::strcmp(fc::error_code_to_string(fc::ErrorCode::kBufferTooSmall), "BufferTooSmall") == 0);
    assert(std::strcmp(fc::error_code_to_string(fc::ErrorCode::kNotSupported), "NotSupported") == 0);
    assert(std::strcmp(fc::error_code_to_string(fc::ErrorCode::kTimeout), "Timeout") == 0);

    std::printf("PASS: error_code_to_string core errors\n");
}

void test_error_code_to_string_hardware() {
    assert(std::strcmp(fc::error_code_to_string(fc::ErrorCode::kSensorInvalidData), "SensorInvalidData") == 0);
    assert(std::strcmp(fc::error_code_to_string(fc::ErrorCode::kSensorTimeout), "SensorTimeout") == 0);
    assert(std::strcmp(fc::error_code_to_string(fc::ErrorCode::kCommLinkDown), "CommLinkDown") == 0);
    assert(std::strcmp(fc::error_code_to_string(fc::ErrorCode::kPowerFault), "PowerFault") == 0);

    std::printf("PASS: error_code_to_string hardware errors\n");
}

void test_error_code_to_string_math() {
    assert(std::strcmp(fc::error_code_to_string(fc::ErrorCode::kDivisionByZero), "DivisionByZero") == 0);
    assert(std::strcmp(fc::error_code_to_string(fc::ErrorCode::kOverflow), "Overflow") == 0);
    assert(std::strcmp(fc::error_code_to_string(fc::ErrorCode::kNaN), "NaN") == 0);

    std::printf("PASS: error_code_to_string math errors\n");
}

// ============================================================================
// Error Code Grouping Tests
// ============================================================================

void test_error_code_groups() {
    // Core group: 0x0100 - 0x01FF
    assert(static_cast<fc::u16>(fc::ErrorCode::kInvalidArgument) >= 0x0100);
    assert(static_cast<fc::u16>(fc::ErrorCode::kInvalidArgument) <= 0x01FF);

    // Math group: 0x0200 - 0x02FF
    assert(static_cast<fc::u16>(fc::ErrorCode::kDivisionByZero) >= 0x0200);
    assert(static_cast<fc::u16>(fc::ErrorCode::kDivisionByZero) <= 0x02FF);

    // Hardware group: 0x0300 - 0x03FF
    assert(static_cast<fc::u16>(fc::ErrorCode::kSensorTimeout) >= 0x0300);
    assert(static_cast<fc::u16>(fc::ErrorCode::kSensorTimeout) <= 0x03FF);

    // Navigation group: 0x0400 - 0x04FF
    assert(static_cast<fc::u16>(fc::ErrorCode::kGPSNoFix) >= 0x0400);
    assert(static_cast<fc::u16>(fc::ErrorCode::kGPSNoFix) <= 0x04FF);

    // Control group: 0x0500 - 0x05FF
    assert(static_cast<fc::u16>(fc::ErrorCode::kInvalidGain) >= 0x0500);
    assert(static_cast<fc::u16>(fc::ErrorCode::kInvalidGain) <= 0x05FF);

    // System group: 0x0600 - 0x06FF
    assert(static_cast<fc::u16>(fc::ErrorCode::kDeadlineMissed) >= 0x0600);
    assert(static_cast<fc::u16>(fc::ErrorCode::kDeadlineMissed) <= 0x06FF);

    // Telemetry group: 0x0700 - 0x07FF
    assert(static_cast<fc::u16>(fc::ErrorCode::kPacketEncodeError) >= 0x0700);
    assert(static_cast<fc::u16>(fc::ErrorCode::kPacketEncodeError) <= 0x07FF);

    std::printf("PASS: error code groups\n");
}

// ============================================================================
// Error Code Enum Size Tests
// ============================================================================

void test_error_code_size() {
    static_assert(sizeof(fc::ErrorCode) == 2, "ErrorCode must be 2 bytes");

    std::printf("PASS: ErrorCode size is 2 bytes\n");
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::printf("Running error code tests...\n");

    test_ok_is_zero();
    test_is_ok();
    test_is_error();
    test_error_code_to_string_success();
    test_error_code_to_string_core();
    test_error_code_to_string_hardware();
    test_error_code_to_string_math();
    test_error_code_groups();
    test_error_code_size();

    std::printf("All error code tests passed.\n");
    return 0;
}
