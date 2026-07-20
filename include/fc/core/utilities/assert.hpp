/// @file assert.hpp
/// @brief Compile-time and runtime assertion macros for the flight computer.
///
/// This header provides assertion mechanisms that verify developer assumptions
/// during development and testing. Assertions can be disabled in production
/// builds for performance.
///
/// Usage:
///   FC_ASSERT(x >= 0, "x must be non-negative");
///   FC_ASSERT_BOUNDS(index, count);
///   FC_ASSERT_NOT_NULL(ptr);
///
/// Compliance:
///   - Compile-time assertions via static_assert
///   - Runtime assertions via FC_ASSERT macro
///   - Assertions are disabled in production (FC NDEBUG)
///   - Assertions never cause undefined behavior
///
/// @defgroup core_assert Assertions
/// @{

#pragma once

#include "../compiler.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>

// ============================================================================
// Configuration
// ============================================================================

/// @def FC_NDEBUG
/// Define to disable runtime assertions. Do not define in debug builds.

// ============================================================================
// Runtime Assertions
// ============================================================================

#if defined(FC_NDEBUG)

    /// @brief Runtime assertion. No-op in release builds.
    #define FC_ASSERT(condition, ...) ((void)0)

    /// @brief Unreachable code marker. No-op in release builds.
    #define FC_UNREACHABLE_MSG(msg) FC_UNREACHABLE

#else

    /// @brief Runtime assertion that prints file, line, and message on failure.
    ///
    /// @param condition The condition to check. If false, the assertion fails.
    /// @param ... Optional format string and arguments for the error message.
    ///
    /// Behavior on failure:
    ///   1. Prints file, line, function name, and condition to stderr
    ///   2. Prints the optional message
    ///   3. Calls std::abort()
    ///
    /// Time Complexity: O(1)
    /// WCET: < 1 us (on typical embedded hardware)
    /// Memory: 0 bytes heap allocation
    #define FC_ASSERT(condition, ...) \
        do { \
            if (!(condition)) { \
                std::fprintf(stderr, "ASSERTION FAILED: %s\n", #condition); \
                std::fprintf(stderr, "  File: %s:%d\n", __FILE__, __LINE__); \
                std::fprintf(stderr, "  Function: %s\n", FC_FUNCTION_NAME); \
                __VA_ARGS__ \
                std::abort(); \
            } \
        } while (0)

    /// @brief Marks a point that should never be reached.
    ///
    /// If execution reaches this point, the assertion fails.
    #define FC_UNREACHABLE_MSG(msg) \
        do { \
            std::fprintf(stderr, "UNREACHABLE CODE REACHED: %s\n", msg); \
            std::fprintf(stderr, "  File: %s:%d\n", __FILE__, __LINE__); \
            std::fprintf(stderr, "  Function: %s\n", FC_FUNCTION_NAME); \
            std::abort(); \
        } while (0)

#endif

// ============================================================================
// Common Assertion Patterns
// ============================================================================

/// @brief Asserts that a pointer is not null.
#define FC_ASSERT_NOT_NULL(ptr) \
    FC_ASSERT((ptr) != nullptr, \
        std::fprintf(stderr, "  Pointer '%s' is null\n", #ptr);)

/// @brief Asserts that an index is within bounds [0, count).
#define FC_ASSERT_BOUNDS(index, count) \
    FC_ASSERT((index) >= 0 && (index) < (count), \
        std::fprintf(stderr, "  Index %s=%ld out of bounds [0, %s=%ld)\n", \
            #index, static_cast<long>(index), #count, static_cast<long>(count));)

/// @brief Asserts that a value is in range [min, max].
#define FC_ASSERT_RANGE(value, min_val, max_val) \
    FC_ASSERT((value) >= (min_val) && (value) <= (max_val), \
        std::fprintf(stderr, "  Value %s=%ld out of range [%ld, %ld]\n", \
            #value, static_cast<long>(value), \
            static_cast<long>(min_val), static_cast<long>(max_val));)

/// @brief Asserts that a condition is true at compile time.
#define FC_STATIC_ASSERT(condition) \
    static_assert(condition, #condition)

/// @brief Asserts that a condition is true at compile time with a custom message.
#define FC_STATIC_ASSERT_MSG(condition, message) \
    static_assert(condition, message)

// ============================================================================
// Numerical Assertions
// ============================================================================

#if !defined(FC_NDEBUG)

    /// @brief Asserts that a floating-point value is not NaN.
    #define FC_ASSERT_NOT_NAN(value) \
        FC_ASSERT(!std::isnan(value), \
            std::fprintf(stderr, "  Value is NaN: %s\n", #value);)

    /// @brief Asserts that a floating-point value is not infinite.
    #define FC_ASSERT_NOT_INF(value) \
        FC_ASSERT(!std::isinf(value), \
            std::fprintf(stderr, "  Value is infinite: %s\n", #value);)

    /// @brief Asserts that a floating-point value is finite (not NaN or Inf).
    #define FC_ASSERT_FINITE(value) \
        do { \
            FC_ASSERT_NOT_NAN(value); \
            FC_ASSERT_NOT_INF(value); \
        } while (0)

    /// @brief Asserts that a floating-point value is approximately equal to expected.
    ///
    /// @param actual The actual value.
    /// @param expected The expected value.
    /// @param tolerance The maximum allowed absolute difference.
    #define FC_ASSERT_NEAR(actual, expected, tolerance) \
        FC_ASSERT(std::abs((actual) - (expected)) <= (tolerance), \
            std::fprintf(stderr, "  |%s - %s| > %g\n", \
                #actual, #expected, \
                static_cast<double>(tolerance));)

#else

    #define FC_ASSERT_NOT_NAN(value) ((void)0)
    #define FC_ASSERT_NOT_INF(value) ((void)0)
    #define FC_ASSERT_FINITE(value) ((void)0)
    #define FC_ASSERT_NEAR(actual, expected, tolerance) ((void)0)

#endif

/// @}
