/// @file types.hpp
/// @brief Fixed-width type aliases for the flight computer.
///
/// All hardware-facing interfaces shall exclusively use these fixed-width
/// types. The bare types (int, long, unsigned) are prohibited in hardware
/// interfaces per the coding standard.
///
/// Usage:
///   Use fc::u32 instead of unsigned int.
///   Use fc::i16 instead of short.
///   Use fc::f32 instead of float.
///
/// Compliance:
///   - Fixed-width types from <cstdint>
///   - No platform-specific assumptions
///   - Endianness is explicit where relevant
///
/// @defgroup core_types Fixed-Width Types
/// @{

#pragma once

#include <cstdint>
#include <cstddef>
#include <limits>
#include <type_traits>

namespace fc {

// ============================================================================
// Signed Integer Types
// ============================================================================

/// @brief 8-bit signed integer. Range: [-128, 127].
using i8 = std::int8_t;

/// @brief 16-bit signed integer. Range: [-32768, 32767].
using i16 = std::int16_t;

/// @brief 32-bit signed integer. Range: [-2^31, 2^31-1].
using i32 = std::int32_t;

/// @brief 64-bit signed integer. Range: [-2^63, 2^63-1].
using i64 = std::int64_t;

// ============================================================================
// Unsigned Integer Types
// ============================================================================

/// @brief 8-bit unsigned integer. Range: [0, 255].
using u8 = std::uint8_t;

/// @brief 16-bit unsigned integer. Range: [0, 65535].
using u16 = std::uint16_t;

/// @brief 32-bit unsigned integer. Range: [0, 2^32-1].
using u32 = std::uint32_t;

/// @brief 64-bit unsigned integer. Range: [0, 2^64-1].
using u64 = std::uint64_t;

// ============================================================================
// Floating-Point Types
// ============================================================================

/// @brief 32-bit IEEE 754 floating-point. ~7 significant digits.
using f32 = float;

/// @brief 64-bit IEEE 754 floating-point. ~15 significant digits.
using f64 = double;

// ============================================================================
// Size and Index Types
// ============================================================================

/// @brief Byte type for raw memory operations.
using byte = std::uint8_t;

/// @brief Size type for container sizes and indices. Matches std::size_t.
using usize = std::size_t;

/// @brief Signed size type for difference calculations.
using isize = std::ptrdiff_t;

// ============================================================================
// Type Traits
// ============================================================================

/// @brief Trait to check if a type is one of the fc integer types.
template <typename T>
struct is_fc_integer : std::disjunction<
    std::is_same<T, i8>,
    std::is_same<T, i16>,
    std::is_same<T, i32>,
    std::is_same<T, i64>,
    std::is_same<T, u8>,
    std::is_same<T, u16>,
    std::is_same<T, u32>,
    std::is_same<T, u64>
> {};

/// @brief Helper variable template for is_fc_integer.
template <typename T>
inline constexpr bool is_fc_integer_v = is_fc_integer<T>::value;

/// @brief Trait to check if a type is one of the fc floating-point types.
template <typename T>
struct is_fc_floating_point : std::disjunction<
    std::is_same<T, f32>,
    std::is_same<T, f64>
> {};

/// @brief Helper variable template for is_fc_floating_point.
template <typename T>
inline constexpr bool is_fc_floating_point_v = is_fc_floating_point<T>::value;

/// @brief Trait to check if a type is any fc numeric type.
template <typename T>
struct is_fc_numeric : std::disjunction<
    is_fc_integer<T>,
    is_fc_floating_point<T>
> {};

/// @brief Helper variable template for is_fc_numeric.
template <typename T>
inline constexpr bool is_fc_numeric_v = is_fc_numeric<T>::value;

// ============================================================================
// Numeric Limits Helpers
// ============================================================================

/// @brief Maximum value for a given fc type.
template <typename T>
[[nodiscard]] constexpr T numeric_max() noexcept {
    static_assert(is_fc_numeric_v<T>, "T must be an fc numeric type");
    return std::numeric_limits<T>::max();
}

/// @brief Minimum value for a given fc type.
template <typename T>
[[nodiscard]] constexpr T numeric_min() noexcept {
    static_assert(is_fc_numeric_v<T>, "T must be an fc numeric type");
    return std::numeric_limits<T>::lowest();
}

/// @brief Epsilon (machine precision) for floating-point types.
template <typename T>
[[nodiscard]] constexpr T numeric_epsilon() noexcept {
    static_assert(is_fc_floating_point_v<T>, "T must be an fc floating-point type");
    return std::numeric_limits<T>::epsilon();
}

} // namespace fc

/// @}
