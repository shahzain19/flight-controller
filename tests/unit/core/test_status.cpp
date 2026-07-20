/// @file test_status.cpp
/// @brief Unit tests for fc::Status class.

#include <fc/core/status/status.hpp>

#include <cassert>
#include <cstdio>
#include <cstring>

// ============================================================================
// Construction Tests
// ============================================================================

void test_default_constructor() {
    fc::Status s;
    assert(s.is_ok());
    assert(s.code() == fc::ErrorCode::kOk);
    assert(s.message() == nullptr);
    assert(s.code_string() != nullptr);
    assert(std::strcmp(s.code_string(), "Ok") == 0);

    std::printf("PASS: default constructor\n");
}

void test_error_code_constructor() {
    fc::Status s(fc::ErrorCode::kInvalidArgument);
    assert(!s.is_ok());
    assert(s.code() == fc::ErrorCode::kInvalidArgument);
    assert(s.message() == nullptr);
    assert(std::strcmp(s.code_string(), "InvalidArgument") == 0);

    std::printf("PASS: error code constructor\n");
}

void test_error_code_message_constructor() {
    fc::Status s(fc::ErrorCode::kTimeout, "timed out");
    assert(!s.is_ok());
    assert(s.code() == fc::ErrorCode::kTimeout);
    assert(s.message() != nullptr);
    assert(std::strcmp(s.message(), "timed out") == 0);
    assert(std::strcmp(s.code_string(), "Timeout") == 0);

    std::printf("PASS: error code + message constructor\n");
}

// ============================================================================
// Factory Method Tests
// ============================================================================

void test_ok_factory() {
    [[maybe_unused]] fc::Status s = fc::Status::ok();
    assert(s.is_ok());
    assert(s.code() == fc::ErrorCode::kOk);

    std::printf("PASS: ok factory\n");
}

void test_error_factory() {
    [[maybe_unused]] fc::Status s = fc::Status::error(fc::ErrorCode::kDivisionByZero, "divide by zero");
    assert(!s.is_ok());
    assert(s.code() == fc::ErrorCode::kDivisionByZero);
    assert(std::strcmp(s.message(), "divide by zero") == 0);

    std::printf("PASS: error factory\n");
}

// ============================================================================
// Observer Tests
// ============================================================================

void test_is_error() {
    fc::Status ok_status;
    fc::Status error_status(fc::ErrorCode::kOutOfBounds);

    assert(!ok_status.is_error());
    assert(error_status.is_error());

    std::printf("PASS: is_error\n");
}

// ============================================================================
// Comparison Operator Tests
// ============================================================================

void test_equality() {
    fc::Status s1(fc::ErrorCode::kTimeout);
    fc::Status s2(fc::ErrorCode::kTimeout);
    fc::Status s3(fc::ErrorCode::kInvalidArgument);

    assert(s1 == s2);
    assert(s1 != s3);

    std::printf("PASS: equality operators\n");
}

void test_error_code_comparison() {
    fc::Status s(fc::ErrorCode::kTimeout);

    assert(s == fc::ErrorCode::kTimeout);
    assert(s != fc::ErrorCode::kInvalidArgument);

    std::printf("PASS: ErrorCode comparison\n");
}

// ============================================================================
// Boolean Conversion Tests
// ============================================================================

void test_bool_conversion() {
    fc::Status ok_status;
    fc::Status error_status(fc::ErrorCode::kSensorTimeout);

    if (ok_status) {
        // ok_status is OK
    } else {
        assert(false);  // Should not reach here
    }

    if (!error_status) {
        // error_status is an error
    } else {
        assert(false);  // Should not reach here
    }

    std::printf("PASS: bool conversion\n");
}

// ============================================================================
// Copy Semantics Tests
// ============================================================================

void test_copy_constructor() {
    fc::Status original(fc::ErrorCode::kTimeout, "original");
    [[maybe_unused]] fc::Status copy = original;

    assert(copy == original);
    assert(copy.code() == original.code());
    assert(std::strcmp(copy.message(), original.message()) == 0);

    std::printf("PASS: copy constructor\n");
}

void test_copy_assignment() {
    fc::Status s1(fc::ErrorCode::kTimeout);
    fc::Status s2;
    s2 = s1;

    assert(s2 == s1);

    std::printf("PASS: copy assignment\n");
}

// ============================================================================
// Free Function Tests
// ============================================================================

void test_free_is_ok() {
    fc::Status ok_status;
    fc::Status error_status(fc::ErrorCode::kTimeout);

    assert(fc::is_ok(ok_status));
    assert(!fc::is_ok(error_status));

    std::printf("PASS: free is_ok function\n");
}

void test_free_is_error() {
    fc::Status ok_status;
    fc::Status error_status(fc::ErrorCode::kTimeout);

    assert(!fc::is_error(ok_status));
    assert(fc::is_error(error_status));

    std::printf("PASS: free is_error function\n");
}

// ============================================================================
// Size Tests
// ============================================================================

void test_status_size() {
    // Status should be small: ErrorCode (2 bytes) + pointer (8 bytes) = 16 bytes
    // This is a soft check; actual size depends on alignment
    assert(sizeof(fc::Status) <= 24);

    std::printf("PASS: Status size is %zu bytes\n", sizeof(fc::Status));
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::printf("Running status tests...\n");

    test_default_constructor();
    test_error_code_constructor();
    test_error_code_message_constructor();
    test_ok_factory();
    test_error_factory();
    test_is_error();
    test_equality();
    test_error_code_comparison();
    test_bool_conversion();
    test_copy_constructor();
    test_copy_assignment();
    test_free_is_ok();
    test_free_is_error();
    test_status_size();

    std::printf("All status tests passed.\n");
    return 0;
}
