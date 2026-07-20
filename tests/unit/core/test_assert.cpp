/// @file test_assert.cpp
/// @brief Unit tests for fc assertion macros.

// Note: We test assertions in debug mode (FC_NDEBUG not defined).
// In release mode, assertions are no-ops.

#include <fc/core/types/types.hpp>
#include <fc/core/utilities/assert.hpp>

#include <cassert>
#include <cmath>
#include <cstdio>

// ============================================================================
// Compile-Time Assertion Tests
// ============================================================================

void test_static_assert() {
    FC_STATIC_ASSERT(true);
    FC_STATIC_ASSERT(sizeof(int) >= 4);
    FC_STATIC_ASSERT_MSG(sizeof(fc::f64) == 8, "f64 must be 8 bytes");

    std::printf("PASS: static_assert\n");
}

// ============================================================================
// Runtime Assertion Tests
// ============================================================================

#if !defined(FC_NDEBUG)

void test_assert_true() {
    FC_ASSERT(true);
    FC_ASSERT(1 == 1);
    FC_ASSERT(sizeof(fc::u32) == 4);

    std::printf("PASS: FC_ASSERT(true)\n");
}

void test_assert_not_null() {
    fc::i32 x = 42;
    FC_ASSERT_NOT_NULL(&x);

    std::printf("PASS: FC_ASSERT_NOT_NULL\n");
}

void test_assert_bounds() {
    FC_ASSERT_BOUNDS(0, 5);
    FC_ASSERT_BOUNDS(4, 5);

    std::printf("PASS: FC_ASSERT_BOUNDS\n");
}

void test_assert_range() {
    fc::i32 x = 50;
    FC_ASSERT_RANGE(x, 0, 100);

    std::printf("PASS: FC_ASSERT_RANGE\n");
}

// ============================================================================
// Numerical Assertion Tests
// ============================================================================

void test_assert_not_nan() {
    fc::f64 x = 1.0;
    FC_ASSERT_NOT_NAN(x);

    std::printf("PASS: FC_ASSERT_NOT_NAN\n");
}

void test_assert_not_inf() {
    fc::f64 x = 1.0;
    FC_ASSERT_NOT_INF(x);

    std::printf("PASS: FC_ASSERT_NOT_INF\n");
}

void test_assert_finite() {
    fc::f64 x = 1.0;
    FC_ASSERT_FINITE(x);

    std::printf("PASS: FC_ASSERT_FINITE\n");
}

void test_assert_near() {
    fc::f64 actual = 1.001;
    fc::f64 expected = 1.0;
    fc::f64 tolerance = 0.01;
    FC_ASSERT_NEAR(actual, expected, tolerance);

    std::printf("PASS: FC_ASSERT_NEAR\n");
}

#endif

// ============================================================================
// Main
// ============================================================================

int main() {
    std::printf("Running assert tests...\n");

    test_static_assert();

#if !defined(FC_NDEBUG)
    test_assert_true();
    test_assert_not_null();
    test_assert_bounds();
    test_assert_range();
    test_assert_not_nan();
    test_assert_not_inf();
    test_assert_finite();
    test_assert_near();
#endif

    std::printf("All assert tests passed.\n");
    return 0;
}
