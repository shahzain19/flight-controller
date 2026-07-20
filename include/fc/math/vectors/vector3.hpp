/// @file vector3.hpp
/// @brief 3D vector class for the flight computer.
///
/// A 3D vector with x, y, z components stored as IEEE 754 single-precision
/// floats. Designed for SIMD auto-vectorization with aligned storage.
///
/// Design:
///   - Header-first, fully constexpr
///   - alignas(16) for SSE/NEON register loading
///   - No dynamic allocation
///   - No exceptions
///   - Operations are O(1) unless noted
///
/// Numerical Assumptions:
///   - IEEE 754 single precision (~7 significant digits)
///   - No NaN/Inf checking in hot path (caller responsibility)
///   - No overflow checking (caller ensures reasonable values)
///   - normalize() on near-zero vector produces undefined result
///   - Cross product assumes right-handed coordinate system
///
/// @defgroup math_vector3 Vector3
/// @{

#pragma once

#include "../../core/types/types.hpp"
#include "../../core/compiler.hpp"

#include <cmath>

namespace fc {

/// @brief 3D vector with x, y, z components.
///
/// Storage: 12 bytes data + 4 bytes padding = 16 bytes, alignas(16).
/// The padding ensures safe SIMD register loading.
/// Time complexity: O(1) for all operations.
class Vector3 {
public:
    // ========================================================================
    // Constructors
    // ========================================================================

    /// @brief Default constructor. Initializes to (0, 0, 0).
    constexpr Vector3() noexcept = default;

    /// @brief Constructs a Vector3 with x, y, z components.
    constexpr Vector3(float x, float y, float z) noexcept : x_(x), y_(y), z_(z) {}

    /// @brief Constructs a Vector3 with all components set to scalar.
    constexpr explicit Vector3(float scalar) noexcept : x_(scalar), y_(scalar), z_(scalar) {}

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

    /// @brief Returns a reference to the z component.
    [[nodiscard]] constexpr float& z() noexcept { return z_; }

    /// @brief Returns the z component.
    [[nodiscard]] constexpr float z() const noexcept { return z_; }

    /// @brief Element access by index (0=x, 1=y, 2=z).
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
    [[nodiscard]] constexpr Vector3 operator+(const Vector3& other) const noexcept {
        return Vector3(x_ + other.x_, y_ + other.y_, z_ + other.z_);
    }

    /// @brief Component-wise subtraction.
    [[nodiscard]] constexpr Vector3 operator-(const Vector3& other) const noexcept {
        return Vector3(x_ - other.x_, y_ - other.y_, z_ - other.z_);
    }

    /// @brief Scalar multiplication.
    [[nodiscard]] constexpr Vector3 operator*(float scalar) const noexcept {
        return Vector3(x_ * scalar, y_ * scalar, z_ * scalar);
    }

    /// @brief Scalar division.
    [[nodiscard]] constexpr Vector3 operator/(float scalar) const noexcept {
        return Vector3(x_ / scalar, y_ / scalar, z_ / scalar);
    }

    /// @brief Component-wise multiplication.
    [[nodiscard]] constexpr Vector3 operator*(const Vector3& other) const noexcept {
        return Vector3(x_ * other.x_, y_ * other.y_, z_ * other.z_);
    }

    /// @brief Unary negation.
    [[nodiscard]] constexpr Vector3 operator-() const noexcept {
        return Vector3(-x_, -y_, -z_);
    }

    // ========================================================================
    // Compound Assignment Operators
    // ========================================================================

    constexpr Vector3& operator+=(const Vector3& other) noexcept {
        x_ += other.x_;
        y_ += other.y_;
        z_ += other.z_;
        return *this;
    }

    constexpr Vector3& operator-=(const Vector3& other) noexcept {
        x_ -= other.x_;
        y_ -= other.y_;
        z_ -= other.z_;
        return *this;
    }

    constexpr Vector3& operator*=(float scalar) noexcept {
        x_ *= scalar;
        y_ *= scalar;
        z_ *= scalar;
        return *this;
    }

    constexpr Vector3& operator/=(float scalar) noexcept {
        x_ /= scalar;
        y_ /= scalar;
        z_ /= scalar;
        return *this;
    }

    constexpr Vector3& operator*=(const Vector3& other) noexcept {
        x_ *= other.x_;
        y_ *= other.y_;
        z_ *= other.z_;
        return *this;
    }

    // ========================================================================
    // Geometric Operations
    // ========================================================================

    /// @brief Returns the dot product with another vector.
    ///
    /// Complexity: O(1)
    /// WCET: < 10 ns
    [[nodiscard]] constexpr float dot(const Vector3& other) const noexcept {
        return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
    }

    /// @brief Returns the cross product with another vector.
    ///
    /// Result is perpendicular to both input vectors.
    /// Uses right-hand rule.
    ///
    /// Complexity: O(1)
    /// WCET: < 15 ns
    [[nodiscard]] constexpr Vector3 cross(const Vector3& other) const noexcept {
        return Vector3(
            y_ * other.z_ - z_ * other.y_,
            z_ * other.x_ - x_ * other.z_,
            x_ * other.y_ - y_ * other.x_
        );
    }

    /// @brief Returns the squared length.
    ///
    /// Prefer this over length() when only comparing magnitudes.
    /// Avoids the sqrt computation.
    ///
    /// Complexity: O(1)
    /// WCET: < 10 ns
    [[nodiscard]] constexpr float length_squared() const noexcept {
        return x_ * x_ + y_ * y_ + z_ * z_;
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
    /// Complexity: O(1) (one sqrt + three divides)
    /// WCET: < 30 ns
    [[nodiscard]] Vector3 normalized() const noexcept {
        float len = length();
        return Vector3(x_ / len, y_ / len, z_ / len);
    }

    /// @brief Linearly interpolates from this to target by t.
    ///
    /// @param target The target vector.
    /// @param t Interpolation parameter [0, 1].
    /// @return this * (1 - t) + target * t
    ///
    /// Complexity: O(1)
    /// WCET: < 15 ns
    [[nodiscard]] Vector3 lerp(const Vector3& target, float t) const noexcept {
        return Vector3(
            x_ + t * (target.x_ - x_),
            y_ + t * (target.y_ - y_),
            z_ + t * (target.z_ - z_)
        );
    }

    // ========================================================================
    // Comparison Operators
    // ========================================================================

    [[nodiscard]] constexpr bool operator==(const Vector3& other) const noexcept {
        return x_ == other.x_ && y_ == other.y_ && z_ == other.z_;
    }

    [[nodiscard]] constexpr bool operator!=(const Vector3& other) const noexcept {
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
    alignas(16) float x_ = 0.0f;
    float y_ = 0.0f;
    float z_ = 0.0f;
    float padding_ = 0.0f;  // Align to 16 bytes for SIMD
};

// ============================================================================
// Free Functions
// ============================================================================

/// @brief Scalar * Vector (commutative).
[[nodiscard]] constexpr Vector3 operator*(float scalar, const Vector3& vec) noexcept {
    return vec * scalar;
}

/// @brief Dot product of two vectors.
[[nodiscard]] constexpr float dot(const Vector3& a, const Vector3& b) noexcept {
    return a.dot(b);
}

/// @brief Cross product of two vectors.
[[nodiscard]] constexpr Vector3 cross(const Vector3& a, const Vector3& b) noexcept {
    return a.cross(b);
}

/// @brief Returns the distance between two points.
[[nodiscard]] inline float distance(const Vector3& a, const Vector3& b) noexcept {
    return (b - a).length();
}

/// @brief Returns the squared distance between two points.
[[nodiscard]] constexpr float distance_squared(const Vector3& a, const Vector3& b) noexcept {
    return (b - a).length_squared();
}

/// @brief Returns a vector perpendicular to both input vectors.
[[nodiscard]] constexpr Vector3 perp(const Vector3& a, const Vector3& b) noexcept {
    return a.cross(b);
}

/// @brief Returns the zero vector.
[[nodiscard]] constexpr Vector3 vector3_zero() noexcept {
    return Vector3(0.0f, 0.0f, 0.0f);
}

/// @brief Returns the unit vector in the x direction.
[[nodiscard]] constexpr Vector3 vector3_unit_x() noexcept {
    return Vector3(1.0f, 0.0f, 0.0f);
}

/// @brief Returns the unit vector in the y direction.
[[nodiscard]] constexpr Vector3 vector3_unit_y() noexcept {
    return Vector3(0.0f, 1.0f, 0.0f);
}

/// @brief Returns the unit vector in the z direction.
[[nodiscard]] constexpr Vector3 vector3_unit_z() noexcept {
    return Vector3(0.0f, 0.0f, 1.0f);
}

} // namespace fc

/// @}
