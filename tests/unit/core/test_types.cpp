/// @file test_types.cpp
/// @brief Unit tests for fc::types (fixed-width type aliases).

#include <fc/core/types/types.hpp>

#include <cassert>
#include <cstdio>
#include <type_traits>

// ============================================================================
// Type Alias Tests
// ============================================================================

void test_signed_integer_types() {
    static_assert(sizeof(fc::i8) == 1, "i8 must be 1 byte");
    static_assert(sizeof(fc::i16) == 2, "i16 must be 2 bytes");
    static_assert(sizeof(fc::i32) == 4, "i32 must be 4 bytes");
    static_assert(sizeof(fc::i64) == 8, "i64 must be 8 bytes");

    static_assert(std::is_signed_v<fc::i8>, "i8 must be signed");
    static_assert(std::is_signed_v<fc::i16>, "i16 must be signed");
    static_assert(std::is_signed_v<fc::i32>, "i32 must be signed");
    static_assert(std::is_signed_v<fc::i64>, "i64 must be signed");

    assert(fc::i8(-1) < fc::i8(0));
    assert(fc::i16(-1) < fc::i16(0));

    std::printf("PASS: signed integer types\n");
}

void test_unsigned_integer_types() {
    static_assert(sizeof(fc::u8) == 1, "u8 must be 1 byte");
    static_assert(sizeof(fc::u16) == 2, "u16 must be 2 bytes");
    static_assert(sizeof(fc::u32) == 4, "u32 must be 4 bytes");
    static_assert(sizeof(fc::u64) == 8, "u64 must be 8 bytes");

    static_assert(std::is_unsigned_v<fc::u8>, "u8 must be unsigned");
    static_assert(std::is_unsigned_v<fc::u16>, "u16 must be unsigned");
    static_assert(std::is_unsigned_v<fc::u32>, "u32 must be unsigned");
    static_assert(std::is_unsigned_v<fc::u64>, "u64 must be unsigned");

    assert(fc::u8(255) > fc::u8(0));
    assert(fc::u16(65535) > fc::u16(0));

    std::printf("PASS: unsigned integer types\n");
}

void test_floating_point_types() {
    static_assert(sizeof(fc::f32) == 4, "f32 must be 4 bytes");
    static_assert(sizeof(fc::f64) == 8, "f64 must be 8 bytes");

    static_assert(std::is_floating_point_v<fc::f32>, "f32 must be floating point");
    static_assert(std::is_floating_point_v<fc::f64>, "f64 must be floating point");

    assert(1.0f > 0.0f);

    std::printf("PASS: floating point types\n");
}

void test_size_types() {
    static_assert(sizeof(fc::byte) == 1, "byte must be 1 byte");
    static_assert(std::is_unsigned_v<fc::byte>, "byte must be unsigned");
    static_assert(sizeof(fc::usize) == sizeof(std::size_t), "usize must match std::size_t");
    static_assert(sizeof(fc::isize) == sizeof(std::ptrdiff_t), "isize must match std::ptrdiff_t");

    std::printf("PASS: size types\n");
}

// ============================================================================
// Type Trait Tests
// ============================================================================

void test_is_fc_integer() {
    static_assert(fc::is_fc_integer_v<fc::i8>, "i8 is fc integer");
    static_assert(fc::is_fc_integer_v<fc::i16>, "i16 is fc integer");
    static_assert(fc::is_fc_integer_v<fc::i32>, "i32 is fc integer");
    static_assert(fc::is_fc_integer_v<fc::i64>, "i64 is fc integer");
    static_assert(fc::is_fc_integer_v<fc::u8>, "u8 is fc integer");
    static_assert(fc::is_fc_integer_v<fc::u16>, "u16 is fc integer");
    static_assert(fc::is_fc_integer_v<fc::u32>, "u32 is fc integer");
    static_assert(fc::is_fc_integer_v<fc::u64>, "u64 is fc integer");

    static_assert(!fc::is_fc_integer_v<fc::f32>, "f32 is not fc integer");
    static_assert(!fc::is_fc_integer_v<fc::f64>, "f64 is not fc integer");

    std::printf("PASS: is_fc_integer trait\n");
}

void test_is_fc_floating_point() {
    static_assert(fc::is_fc_floating_point_v<fc::f32>, "f32 is fc floating point");
    static_assert(fc::is_fc_floating_point_v<fc::f64>, "f64 is fc floating point");

    static_assert(!fc::is_fc_floating_point_v<fc::i32>, "i32 is not fc floating point");
    static_assert(!fc::is_fc_floating_point_v<fc::u64>, "u64 is not fc floating point");

    std::printf("PASS: is_fc_floating_point trait\n");
}

void test_is_fc_numeric() {
    static_assert(fc::is_fc_numeric_v<fc::i8>, "i8 is fc numeric");
    static_assert(fc::is_fc_numeric_v<fc::f32>, "f32 is fc numeric");
    static_assert(fc::is_fc_numeric_v<fc::f64>, "f64 is fc numeric");
    static_assert(fc::is_fc_numeric_v<fc::u64>, "u64 is fc numeric");

    static_assert(!fc::is_fc_numeric_v<void*>, "void* is not fc numeric");

    std::printf("PASS: is_fc_numeric trait\n");
}

// ============================================================================
// Numeric Limits Tests
// ============================================================================

void test_numeric_max() {
    assert(fc::numeric_max<fc::i8>() == 127);
    assert(fc::numeric_max<fc::i16>() == 32767);
    assert(fc::numeric_max<fc::i32>() == 2147483647);
    assert(fc::numeric_max<fc::u8>() == 255);
    assert(fc::numeric_max<fc::u16>() == 65535);

    std::printf("PASS: numeric_max\n");
}

void test_numeric_min() {
    assert(fc::numeric_min<fc::i8>() == -128);
    assert(fc::numeric_min<fc::i16>() == -32768);
    assert(fc::numeric_min<fc::u8>() == 0);
    assert(fc::numeric_min<fc::u16>() == 0);

    std::printf("PASS: numeric_min\n");
}

void test_numeric_epsilon() {
    assert(fc::numeric_epsilon<fc::f32>() > 0.0f);
    assert(fc::numeric_epsilon<fc::f64>() > 0.0);
    assert(fc::numeric_epsilon<fc::f32>() < 1.0f);
    assert(fc::numeric_epsilon<fc::f64>() < 1.0);

    std::printf("PASS: numeric_epsilon\n");
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::printf("Running type tests...\n");

    test_signed_integer_types();
    test_unsigned_integer_types();
    test_floating_point_types();
    test_size_types();
    test_is_fc_integer();
    test_is_fc_floating_point();
    test_is_fc_numeric();
    test_numeric_max();
    test_numeric_min();
    test_numeric_epsilon();

    std::printf("All type tests passed.\n");
    return 0;
}
