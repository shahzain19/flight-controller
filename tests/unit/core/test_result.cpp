/// @file test_result.cpp
/// @brief Unit tests for fc::Result<T> class.

#include <fc/core/status/result.hpp>

#include <cassert>
#include <cstdio>
#include <cstring>

// ============================================================================
// Helper Types for Testing
// ============================================================================

struct Point {
    fc::f64 x = 0.0;
    fc::f64 y = 0.0;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// ============================================================================
// Construction Tests
// ============================================================================

void test_ok_construction() {
    auto result = fc::Result<fc::i32>::ok(42);
    assert(result.has_value());
    assert(result.value() == 42);

    std::printf("PASS: ok construction (copy)\n");
}

void test_ok_move_construction() {
    fc::i32 val = 42;
    auto result = fc::Result<fc::i32>::ok(std::move(val));
    assert(result.has_value());
    assert(result.value() == 42);

    std::printf("PASS: ok construction (move)\n");
}

void test_error_construction() {
    auto result = fc::Result<fc::i32>::error(fc::ErrorCode::kOutOfBounds);
    assert(!result.has_value());
    assert(result.status().code() == fc::ErrorCode::kOutOfBounds);
    assert(result.error_code() == fc::ErrorCode::kOutOfBounds);

    std::printf("PASS: error construction\n");
}

void test_error_with_status() {
    fc::Status status(fc::ErrorCode::kTimeout, "timed out");
    auto result = fc::Result<fc::i32>::error(status);
    assert(!result.has_value());
    assert(result.status() == status);

    std::printf("PASS: error construction with Status\n");
}

// ============================================================================
// Value Access Tests
// ============================================================================

void test_value_access() {
    auto result = fc::Result<fc::f64>::ok(3.14);
    assert(result.value() == 3.14);
    assert(*result == 3.14);

    std::printf("PASS: value access\n");
}

void test_const_value_access() {
    const auto result = fc::Result<fc::f64>::ok(3.14);
    assert(result.value() == 3.14);
    assert(*result == 3.14);

    std::printf("PASS: const value access\n");
}

void test_pointer_access() {
    fc::i32 val = 42;
    auto result = fc::Result<fc::i32*>::ok(&val);
    assert(result.has_value());
    assert(**result == 42);

    std::printf("PASS: pointer access\n");
}

// ============================================================================
// Boolean Conversion Tests
// ============================================================================

void test_bool_conversion_ok() {
    auto result = fc::Result<fc::i32>::ok(42);

    if (result) {
        // result is OK
    } else {
        assert(false);  // Should not reach here
    }

    std::printf("PASS: bool conversion (ok)\n");
}

void test_bool_conversion_error() {
    auto result = fc::Result<fc::i32>::error(fc::ErrorCode::kTimeout);

    if (!result) {
        // result is an error
    } else {
        assert(false);  // Should not reach here
    }

    std::printf("PASS: bool conversion (error)\n");
}

// ============================================================================
// Copy Semantics Tests
// ============================================================================

void test_copy_constructor() {
    auto original = fc::Result<fc::i32>::ok(42);
    auto copy = original;

    assert(copy.has_value());
    assert(copy.value() == original.value());

    std::printf("PASS: copy constructor (ok)\n");
}

void test_copy_constructor_error() {
    auto original = fc::Result<fc::i32>::error(fc::ErrorCode::kTimeout);
    auto copy = original;

    assert(!copy.has_value());
    assert(copy.error_code() == original.error_code());

    std::printf("PASS: copy constructor (error)\n");
}

void test_copy_assignment() {
    auto result = fc::Result<fc::i32>::ok(42);
    auto assigned = fc::Result<fc::i32>::error(fc::ErrorCode::kTimeout);
    assigned = result;

    assert(assigned.has_value());
    assert(assigned.value() == 42);

    std::printf("PASS: copy assignment\n");
}

// ============================================================================
// Move Semantics Tests
// ============================================================================

void test_move_constructor() {
    auto original = fc::Result<fc::i32>::ok(42);
    auto moved = fc::Result<fc::i32>(std::move(original));

    assert(moved.has_value());
    assert(moved.value() == 42);

    std::printf("PASS: move constructor\n");
}

void test_move_assignment() {
    auto result = fc::Result<fc::i32>::ok(42);
    auto assigned = fc::Result<fc::i32>::error(fc::ErrorCode::kTimeout);
    assigned = fc::Result<fc::i32>(std::move(result));

    assert(assigned.has_value());
    assert(assigned.value() == 42);

    std::printf("PASS: move assignment\n");
}

// ============================================================================
// Complex Type Tests
// ============================================================================

void test_complex_type() {
    Point p{1.0, 2.0};
    auto result = fc::Result<Point>::ok(p);

    assert(result.has_value());
    assert(result.value().x == 1.0);
    assert(result.value().y == 2.0);

    std::printf("PASS: complex type (Point)\n");
}

// ============================================================================
// Equality Operator Tests
// ============================================================================

void test_equality_result_result() {
    auto r1 = fc::Result<fc::i32>::ok(42);
    auto r2 = fc::Result<fc::i32>::ok(42);
    auto r3 = fc::Result<fc::i32>::ok(99);
    auto r4 = fc::Result<fc::i32>::error(fc::ErrorCode::kTimeout);
    auto r5 = fc::Result<fc::i32>::error(fc::ErrorCode::kTimeout);

    assert(r1 == r2);
    assert(r1 != r3);
    assert(r4 == r5);

    std::printf("PASS: Result-Result equality\n");
}

void test_equality_result_errorcode() {
    auto ok_result = fc::Result<fc::i32>::ok(42);
    auto error_result = fc::Result<fc::i32>::error(fc::ErrorCode::kTimeout);

    assert(ok_result == fc::ErrorCode::kOk);
    assert(error_result == fc::ErrorCode::kTimeout);
    assert(error_result != fc::ErrorCode::kOutOfBounds);

    std::printf("PASS: Result-ErrorCode equality\n");
}

// ============================================================================
// Factory Function Tests
// ============================================================================

void test_make_ok() {
    auto result = fc::make_ok(42);
    assert(result.has_value());
    assert(result.value() == 42);

    std::printf("PASS: make_ok\n");
}

void test_make_error() {
    auto result = fc::make_error(fc::ErrorCode::kTimeout);
    assert(!result.has_value());
    assert(result.error_code() == fc::ErrorCode::kTimeout);

    std::printf("PASS: make_error\n");
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::printf("Running result tests...\n");

    test_ok_construction();
    test_ok_move_construction();
    test_error_construction();
    test_error_with_status();
    test_value_access();
    test_const_value_access();
    test_pointer_access();
    test_bool_conversion_ok();
    test_bool_conversion_error();
    test_copy_constructor();
    test_copy_constructor_error();
    test_copy_assignment();
    test_move_constructor();
    test_move_assignment();
    test_complex_type();
    test_equality_result_result();
    test_equality_result_errorcode();
    test_make_ok();
    test_make_error();

    std::printf("All result tests passed.\n");
    return 0;
}
