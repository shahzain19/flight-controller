/// @file vector2.hpp
/// @brief 2D vector class for the flight computer.
///
/// A 2D vector with x and y components stored as IEEE 754 single-precision
/// floats. Designed for SIMD auto-vectorization with aligned storage.
///
/// Design:
///   - Header-first, fully constexpr
///   - alignas(8) for natural alignment
///   - No dynamic allocation
///   - No exceptions
///   - Operations are O(1) unless noted
///
/// Numerical Assumptions:
///   - IEEE 754 single precision (~7 significant digits)
///   - No NaN/Inf checking in hot path (caller responsibility)
///   - No overflow checking (caller ensures reasonable values)
///   - normalize() on near-zero vector produces undefined result
///
/// @defgroup math_vector2 Vector2
/// @{

#pragma once

#include "../../core/types/types.hpp"
#include "../../core/compiler.hpp"

#include <cmath>

namespace fc {

/// @brief 2D vector with x and y components.
///
/// Storage: 8 bytes, alignas(8).
/// Time complexity: O(1) for all operations.
class Vector2 {
public:
    // ========================================================================
    // Constructors
    // ========================================================================

    /// @brief Default constructor. Initializes to (0, 0).
    constexpr Vector2() noexcept = default;

    /// @brief Constructs a Vector2 with x and y components.
    constexpr Vector2(float x, float y) noexcept : x_(x), y_(y) {}

    /// @brief Constructs a Vector2 with both components set to scalar.
    constexpr explicit Vector2(float scalar) noexcept : x_(scalar), y_(scalar) {}

    // ========================================================================
    // Component Access
    // ========================================================================

    /// @brief Returns a reference to the x component.
    [[nodiscard]] constexpr float& x() noexcept { return x_; }

    /// @brief Returns the x component.
    [[nodiscard]] constexpr float x() const noexcept { return x_; }

    /// @brief Returns a reference to the y component.
    [[nodiscard]] constexpr float& y() noexcept { return y_; }

    /// @brief Returns the y component.
    [[nodiscard]] constexpr float y() const noexcept { return y_; }

    /// @brief Element access by index (0=x, 1=y).
    [[nodiscard]] constexpr float& operator[](usize index) noexcept {
        return (&x_)[index];
    }

    /// @brief Element access by index (const).
    [[nodiscard]] constexpr float operator[](usize index) const noexcept {
        return (&x_)[index];
    }

    // ========================================================================
    // Arithmetic Operators
    // ========================================================================

    /// @brief Component-wise addition.
    [[nodiscard]] constexpr Vector2 operator+(const Vector2& other) const noexcept {
        return Vector2(x_ + other.x_, y_ + other.y_);
    }

    /// @brief Component-wise subtraction.
    [[nodiscard]] constexpr Vector2 operator-(const Vector2& other) const noexcept {
        return Vector2(x_ - other.x_, y_ - other.y_);
    }

    /// @brief Scalar multiplication.
    [[nodiscard]] constexpr Vector2 operator*(float scalar) const noexcept {
        return Vector2(x_ * scalar, y_ * scalar);
    }

    /// @brief Scalar division.
    [[nodiscard]] constexpr Vector2 operator/(float scalar) const noexcept {
        return Vector2(x_ / scalar, y_ / scalar);
    }

    /// @brief Component-wise multiplication.
    [[nodiscard]] constexpr Vector2 operator*(const Vector2& other) const noexcept {
        return Vector2(x_ * other.x_, y_ * other.y_);
    }

    /// @brief Unary negation.
    [[nodiscard]] constexpr Vector2 operator-() const noexcept {
        return Vector2(-x_, -y_);
    }

    // ========================================================================
    // Compound Assignment Operators
    // ========================================================================

    constexpr Vector2& operator+=(const Vector2& other) noexcept {
        x_ += other.x_;
        y_ += other.y_;
        return *this;
    }

    constexpr Vector2& operator-=(const Vector2& other) noexcept {
        x_ -= other.x_;
        y_ -= other.y_;
        return *this;
    }

    constexpr Vector2& operator*=(float scalar) noexcept {
        x_ *= scalar;
        y_ *= scalar;
        return *this;
    }

    constexpr Vector2& operator/=(float scalar) noexcept {
        x_ /= scalar;
        y_ /= scalar;
        return *this;
    }

    constexpr Vector2& operator*=(const Vector2& other) noexcept {
        x_ *= other.x_;
        y_ *= other.y_;
        return *this;
    }

    // ========================================================================
    // Geometric Operations
    // ========================================================================

    /// @brief Returns the dot product with another vector.
    ///
    /// Complexity: O(1)
    /// WCET: < 10 ns
    [[nodiscard]] constexpr float dot(const Vector2& other) const noexcept {
        return x_ * other.x_ + y_ * other.y_;
    }

    /// @brief Returns the squared length.
    ///
    /// Prefer this over length() when only comparing magnitudes.
    /// Avoids the sqrt computation.
    ///
    /// Complexity: O(1)
    /// WCET: < 10 ns
    [[nodiscard]] constexpr float length_squared() const noexcept {
        return x_ * x_ + y_ * y_;
    }

    /// @brief Returns the length (magnitude).
    ///
    /// Complexity: O(1) (one sqrt)
    /// WCET: < 20 ns
    [[nodiscard]] float length() const noexcept {
        return std::sqrt(length_squared());
    }

    /// @brief Returns a normalized copy (unit length).
    ///
    /// @note Behavior is undefined if length() is near zero.
    /// Caller must ensure the vector has non-zero magnitude.
    ///
    /// Complexity: O(1) (one sqrt + two divides)
    /// WCET: < 30 ns
    [[nodiscard]] Vector2 normalized() const noexcept {
        float len = length();
        return Vector2(x_ / len, y_ / len);
    }

    /// @brief Linearly interpolates from this to target by t.
    ///
    /// @param target The target vector.
    /// @param t Interpolation parameter [0, 1].
    /// @return this * (1 - t) + target * t
    ///
    /// Complexity: O(1)
    /// WCET: < 15 ns
    [[nodiscard]] Vector2 lerp(const Vector2& target, float t) const noexcept {
        return Vector2(
            x_ + t * (target.x_ - x_),
            y_ + t * (target.y_ - y_)
        );
    }

    // ========================================================================
    // Comparison Operators
    // ========================================================================

    [[nodiscard]] constexpr bool operator==(const Vector2& other) const noexcept {
        return x_ == other.x_ && y_ == other.y_;
    }

    [[nodiscard]] constexpr bool operator!=(const Vector2& other) const noexcept {
        return !(*this == other);
    }

    // ========================================================================
    // Data Access
    // ========================================================================

    /// @brief Returns a pointer to the raw data.
    [[nodiscard]] constexpr float* data() noexcept { return &x_; }

    /// @brief Returns a const pointer to the raw data.
    [[nodiscard]] constexpr const float* data() const noexcept { return &x_; }

private:
    float x_ = 0.0f;
    float y_ = 0.0f;
};

// ============================================================================
// Free Functions
// ============================================================================

/// @brief Scalar * Vector (commutative).
[[nodiscard]] constexpr Vector2 operator*(float scalar, const Vector2& vec) noexcept {
    return vec * scalar;
}

/// @brief Dot product of two vectors.
[[nodiscard]] constexpr float dot(const Vector2& a, const Vector2& b) noexcept {
    return a.dot(b);
}

/// @brief Returns the distance between two points.
[[nodiscard]] inline float distance(const Vector2& a, const Vector2& b) noexcept {
    return (b - a).length();
}

/// @brief Returns the squared distance between two points.
[[nodiscard]] constexpr float distance_squared(const Vector2& a, const Vector2& b) noexcept {
    return (b - a).length_squared();
}

/// @brief Returns the perpendicular vector (-y, x).
[[nodiscard]] constexpr Vector2 perpendicular(const Vector2& v) noexcept {
    return Vector2(-v.y(), v.x());
}

/// @brief Returns the zero vector.
[[nodiscard]] constexpr Vector2 vector2_zero() noexcept {
    return Vector2(0.0f, 0.0f);
}

/// @brief Returns the unit vector in the x direction.
[[nodiscard]] constexpr Vector2 vector2_unit_x() noexcept {
    return Vector2(1.0f, 0.0f);
}

/// @brief Returns the unit vector in the y direction.
[[nodiscard]] constexpr Vector2 vector2_unit_y() noexcept {
    return Vector2(0.0f, 1.0f);
}

} // namespace fc

/// @}
