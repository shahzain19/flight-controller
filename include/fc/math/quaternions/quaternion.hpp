/// @file quaternion.hpp
/// @brief Quaternion class for the flight computer.
///
/// A unit quaternion (Hamilton convention) for representing 3D rotations.
/// Storage: {x, y, z, w} where w is the scalar part.
///
/// Hamilton Convention:
///   q = w + xi + yj + zk
///   q * p = (wp - pw) + (wp + pw + (p x w))  (not commutative)
///
/// Design:
///   - Header-first, fully constexpr where possible
///   - alignas(16) for SIMD register loading
///   - No dynamic allocation
///   - No exceptions
///   - Assumes unit quaternions for rotation operations
///
/// Numerical Assumptions:
///   - IEEE 754 single precision (~7 significant digits)
///   - normalize() produces a unit quaternion (length ≈ 1.0)
///   - slerp() assumes both quaternions represent valid rotations
///   - gimbal lock: Euler angle conversion may be unstable near ±90° pitch
///
/// @defgroup math_quaternion Quaternion
/// @{

#pragma once

#include "../../core/types/types.hpp"
#include "../../core/compiler.hpp"
#include "../vectors/vector3.hpp"
#include "../matrices/matrix3.hpp"
#include "../matrices/matrix4.hpp"

#include <cmath>

namespace fc {

/// @brief Quaternion for 3D rotation representation.
///
/// Storage: 16 bytes (4 floats), alignas(16).
/// Uses Hamilton convention: q = w + xi + yj + zk.
class Quaternion {
public:
    // ========================================================================
    // Constructors
    // ========================================================================

    /// @brief Default constructor. Initializes to identity quaternion (no rotation).
    constexpr Quaternion() noexcept = default;

    /// @brief Constructs a Quaternion from x, y, z, w components.
    constexpr Quaternion(float x, float y, float z, float w) noexcept
        : x_(x), y_(y), z_(z), w_(w) {}

    // ========================================================================
    // Factory Methods
    // ========================================================================

    /// @brief Creates a quaternion from an axis and angle.
    ///
    /// @param axis The rotation axis (should be unit length).
    /// @param angle_rad The rotation angle in radians.
    ///
    /// Complexity: O(1)
    [[nodiscard]] static Quaternion from_axis_angle(const Vector3& axis, float angle_rad) noexcept {
        float half = angle_rad * 0.5f;
        float s = std::sin(half);
        float c = std::cos(half);
        return Quaternion(
            axis.x() * s,
            axis.y() * s,
            axis.z() * s,
            c
        );
    }

    /// @brief Creates a quaternion from Euler angles (ZYX convention).
    ///
    /// Rotation order: first Z, then Y, then X (intrinsic).
    /// This is the standard aerospace convention (yaw, pitch, roll).
    ///
    /// @param roll_rad Rotation around X axis (radians).
    /// @param pitch_rad Rotation around Y axis (radians).
    /// @param yaw_rad Rotation around Z axis (radians).
    ///
    /// Complexity: O(1)
    [[nodiscard]] static Quaternion from_euler(float roll_rad, float pitch_rad, float yaw_rad) noexcept {
        float cr = std::cos(roll_rad * 0.5f);
        float sr = std::sin(roll_rad * 0.5f);
        float cp = std::cos(pitch_rad * 0.5f);
        float sp = std::sin(pitch_rad * 0.5f);
        float cy = std::cos(yaw_rad * 0.5f);
        float sy = std::sin(yaw_rad * 0.5f);

        return Quaternion(
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy,
            cr * cp * cy + sr * sp * sy
        );
    }

    /// @brief Creates a quaternion from a rotation matrix.
    ///
    /// Uses Shepperd's method for numerical stability.
    ///
    /// @param m A rotation matrix (orthogonal, det = 1).
    [[nodiscard]] static Quaternion from_matrix(const Matrix3& m) noexcept {
        float trace = m(0, 0) + m(1, 1) + m(2, 2);

        if (trace > 0.0f) {
            float s = 0.5f / std::sqrt(trace + 1.0f);
            return Quaternion(
                (m(2, 1) - m(1, 2)) * s,
                (m(0, 2) - m(2, 0)) * s,
                (m(1, 0) - m(0, 1)) * s,
                0.25f / s
            );
        } else if (m(0, 0) > m(1, 1) && m(0, 0) > m(2, 2)) {
            float s = 2.0f * std::sqrt(1.0f + m(0, 0) - m(1, 1) - m(2, 2));
            return Quaternion(
                0.25f * s,
                (m(0, 1) + m(1, 0)) / s,
                (m(0, 2) + m(2, 0)) / s,
                (m(2, 1) - m(1, 2)) / s
            );
        } else if (m(1, 1) > m(2, 2)) {
            float s = 2.0f * std::sqrt(1.0f + m(1, 1) - m(0, 0) - m(2, 2));
            return Quaternion(
                (m(0, 1) + m(1, 0)) / s,
                0.25f * s,
                (m(1, 2) + m(2, 1)) / s,
                (m(0, 2) - m(2, 0)) / s
            );
        } else {
            float s = 2.0f * std::sqrt(1.0f + m(2, 2) - m(0, 0) - m(1, 1));
            return Quaternion(
                (m(0, 2) + m(2, 0)) / s,
                (m(1, 2) + m(2, 1)) / s,
                0.25f * s,
                (m(1, 0) - m(0, 1)) / s
            );
        }
    }

    // ========================================================================
    // Component Access
    // ========================================================================

    [[nodiscard]] constexpr float& x() noexcept { return x_; }
    [[nodiscard]] constexpr float x() const noexcept { return x_; }
    [[nodiscard]] constexpr float& y() noexcept { return y_; }
    [[nodiscard]] constexpr float y() const noexcept { return y_; }
    [[nodiscard]] constexpr float& z() noexcept { return z_; }
    [[nodiscard]] constexpr float z() const noexcept { return z_; }
    [[nodiscard]] constexpr float& w() noexcept { return w_; }
    [[nodiscard]] constexpr float w() const noexcept { return w_; }

    /// @brief Element access by index (0=x, 1=y, 2=z, 3=w).
    [[nodiscard]] constexpr float& operator[](usize index) noexcept {
        return (&x_)[index];
    }

    /// @brief Element access by index (const).
    [[nodiscard]] constexpr float operator[](usize index) const noexcept {
        return (&x_)[index];
    }

    // ========================================================================
    // Quaternion Arithmetic
    // ========================================================================

    /// @brief Quaternion multiplication (composition of rotations).
    ///
    /// @note Quaternion multiplication is NOT commutative: q * p != p * q
    ///
    /// Complexity: O(1) (16 multiplications, 12 additions)
    /// WCET: < 50 ns
    [[nodiscard]] constexpr Quaternion operator*(const Quaternion& q) const noexcept {
        return Quaternion(
            w_ * q.x_ + x_ * q.w_ + y_ * q.z_ - z_ * q.y_,
            w_ * q.y_ - x_ * q.z_ + y_ * q.w_ + z_ * q.x_,
            w_ * q.z_ + x_ * q.y_ - y_ * q.x_ + z_ * q.w_,
            w_ * q.w_ - x_ * q.x_ - y_ * q.y_ - z_ * q.z_
        );
    }

    /// @brief Quaternion addition.
    [[nodiscard]] constexpr Quaternion operator+(const Quaternion& q) const noexcept {
        return Quaternion(x_ + q.x_, y_ + q.y_, z_ + q.z_, w_ + q.w_);
    }

    /// @brief Quaternion subtraction.
    [[nodiscard]] constexpr Quaternion operator-(const Quaternion& q) const noexcept {
        return Quaternion(x_ - q.x_, y_ - q.y_, z_ - q.z_, w_ - q.w_);
    }

    /// @brief Scalar multiplication.
    [[nodiscard]] constexpr Quaternion operator*(float scalar) const noexcept {
        return Quaternion(x_ * scalar, y_ * scalar, z_ * scalar, w_ * scalar);
    }

    /// @brief Unary negation (represents the same rotation).
    [[nodiscard]] constexpr Quaternion operator-() const noexcept {
        return Quaternion(-x_, -y_, -z_, -w_);
    }

    // ========================================================================
    // Quaternion Operations
    // ========================================================================

    /// @brief Returns the squared length.
    [[nodiscard]] constexpr float length_squared() const noexcept {
        return x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_;
    }

    /// @brief Returns the length.
    [[nodiscard]] float length() const noexcept {
        return std::sqrt(length_squared());
    }

    /// @brief Returns a normalized (unit) quaternion.
    ///
    /// @note Caller must ensure the quaternion is not zero-length.
    [[nodiscard]] Quaternion normalized() const noexcept {
        float len = length();
        float inv_len = 1.0f / len;
        return Quaternion(x_ * inv_len, y_ * inv_len, z_ * inv_len, w_ * inv_len);
    }

    /// @brief Returns the conjugate (inverse for unit quaternions).
    ///
    /// For unit quaternions: conjugate() == inverse()
    ///
    /// Complexity: O(1)
    /// WCET: < 5 ns
    [[nodiscard]] constexpr Quaternion conjugate() const noexcept {
        return Quaternion(-x_, -y_, -z_, w_);
    }

    /// @brief Returns the inverse.
    ///
    /// For unit quaternions, this is equivalent to conjugate().
    /// For non-unit quaternions, this computes the proper inverse.
    ///
    /// Complexity: O(1)
    /// WCET: < 20 ns
    [[nodiscard]] Quaternion inverse() const noexcept {
        float inv_len_sq = 1.0f / length_squared();
        return Quaternion(
            -x_ * inv_len_sq,
            -y_ * inv_len_sq,
            -z_ * inv_len_sq,
            w_ * inv_len_sq
        );
    }

    /// @brief Rotates a vector by this quaternion.
    ///
    /// Uses the efficient formula: v' = q * v * q^-1
    /// For unit quaternions: v' = v + 2w(q.xyz x v) + 2(q.xyz x (q.xyz x v))
    ///
    /// Complexity: O(1)
    /// WCET: < 80 ns
    [[nodiscard]] Vector3 rotate(const Vector3& v) const noexcept {
        Vector3 qv(x_, y_, z_);
        Vector3 t = qv.cross(v) * 2.0f;
        return v + t * w_ + qv.cross(t);
    }

    /// @brief Spherical linear interpolation between two quaternions.
    ///
    /// @param target The target quaternion.
    /// @param t Interpolation parameter [0, 1].
    /// @return Interpolated quaternion.
    ///
    /// @note Assumes both quaternions represent valid rotations.
    /// Uses shortest path interpolation (flips if dot product is negative).
    ///
    /// Complexity: O(1)
    /// WCET: < 100 ns
    [[nodiscard]] Quaternion slerp(const Quaternion& target, float t) const noexcept {
        float dot = x_ * target.x_ + y_ * target.y_ + z_ * target.z_ + w_ * target.w_;

        Quaternion target_adj = target;

        // Take shortest path
        if (dot < 0.0f) {
            target_adj = -target_adj;
            dot = -dot;
        }

        // If quaternions are very close, use linear interpolation
        if (dot > 0.9995f) {
            Quaternion result = *this + (target_adj - *this) * t;
            return result.normalized();
        }

        float theta = std::acos(dot);
        float sin_theta = std::sin(theta);
        float a = std::sin((1.0f - t) * theta) / sin_theta;
        float b = std::sin(t * theta) / sin_theta;

        return Quaternion(
            x_ * a + target_adj.x_ * b,
            y_ * a + target_adj.y_ * b,
            z_ * a + target_adj.z_ * b,
            w_ * a + target_adj.w_ * b
        );
    }

    /// @brief Converts to axis-angle representation.
    ///
    /// @param axis Output: the rotation axis (unit length).
    /// @param angle_rad Output: the rotation angle in radians.
    ///
    /// @note If the quaternion is identity, axis is set to (0, 0, 1) and angle to 0.
    void to_axis_angle(Vector3& axis, float& angle_rad) const noexcept {
        float len_sq = x_ * x_ + y_ * y_ + z_ * z_;

        if (len_sq < 1e-10f) {
            axis = Vector3(0.0f, 0.0f, 1.0f);
            angle_rad = 0.0f;
            return;
        }

        float inv_len = 1.0f / std::sqrt(len_sq);
        angle_rad = 2.0f * std::acos(w_);
        axis = Vector3(x_ * inv_len, y_ * inv_len, z_ * inv_len);
    }

    /// @brief Converts to Euler angles (ZYX convention: yaw, pitch, roll).
    ///
    /// @param roll_rad Output: rotation around X axis.
    /// @param pitch_rad Output: rotation around Y axis.
    /// @param yaw_rad Output: rotation around Z axis.
    ///
    /// @note Near ±90° pitch, the conversion is numerically unstable (gimbal lock).
    void to_euler(float& roll_rad, float& pitch_rad, float& yaw_rad) const noexcept {
        float sinr_cosp = 2.0f * (w_ * x_ + y_ * z_);
        float cosr_cosp = 1.0f - 2.0f * (x_ * x_ + y_ * y_);
        roll_rad = std::atan2(sinr_cosp, cosr_cosp);

        float sinp = 2.0f * (w_ * y_ - z_ * x_);
        if (std::abs(sinp) >= 1.0f) {
            pitch_rad = std::copysign(3.14159265f / 2.0f, sinp);
        } else {
            pitch_rad = std::asin(sinp);
        }

        float siny_cosp = 2.0f * (w_ * z_ + x_ * y_);
        float cosy_cosp = 1.0f - 2.0f * (y_ * y_ + z_ * z_);
        yaw_rad = std::atan2(siny_cosp, cosy_cosp);
    }

    /// @brief Converts to a 3x3 rotation matrix.
    ///
    /// Complexity: O(1)
    /// WCET: < 100 ns
    [[nodiscard]] constexpr Matrix3 to_matrix3() const noexcept {
        float xx = x_ * x_;
        float yy = y_ * y_;
        float zz = z_ * z_;
        float xy = x_ * y_;
        float xz = x_ * z_;
        float yz = y_ * z_;
        float wx = w_ * x_;
        float wy = w_ * y_;
        float wz = w_ * z_;

        return Matrix3(
            1.0f - 2.0f * (yy + zz), 2.0f * (xy - wz),       2.0f * (xz + wy),
            2.0f * (xy + wz),       1.0f - 2.0f * (xx + zz), 2.0f * (yz - wx),
            2.0f * (xz - wy),       2.0f * (yz + wx),       1.0f - 2.0f * (xx + yy)
        );
    }

    /// @brief Converts to a 4x4 rotation matrix (homogeneous).
    ///
    /// Complexity: O(1)
    /// WCET: < 100 ns
    [[nodiscard]] constexpr Matrix4 to_matrix4() const noexcept {
        Matrix3 m3 = to_matrix3();
        return Matrix4(
            m3(0, 0), m3(0, 1), m3(0, 2), 0.0f,
            m3(1, 0), m3(1, 1), m3(1, 2), 0.0f,
            m3(2, 0), m3(2, 1), m3(2, 2), 0.0f,
            0.0f,     0.0f,     0.0f,     1.0f
        );
    }

    // ========================================================================
    // Comparison
    // ========================================================================

    [[nodiscard]] constexpr bool operator==(const Quaternion& other) const noexcept {
        return x_ == other.x_ && y_ == other.y_ && z_ == other.z_ && w_ == other.w_;
    }

    [[nodiscard]] constexpr bool operator!=(const Quaternion& other) const noexcept {
        return !(*this == other);
    }

    // ========================================================================
    // Data Access
    // ========================================================================

    [[nodiscard]] constexpr float* data() noexcept { return &x_; }
    [[nodiscard]] constexpr const float* data() const noexcept { return &x_; }

private:
    alignas(16) float x_ = 0.0f;
    float y_ = 0.0f;
    float z_ = 0.0f;
    float w_ = 1.0f;  // Identity quaternion: (0, 0, 0, 1)
};

// ============================================================================
// Free Functions
// ============================================================================

/// @brief Scalar * Quaternion.
[[nodiscard]] constexpr Quaternion operator*(float scalar, const Quaternion& q) noexcept {
    return q * scalar;
}

/// @brief Conjugate of a quaternion.
[[nodiscard]] constexpr Quaternion conjugate(const Quaternion& q) noexcept {
    return q.conjugate();
}

/// @brief Inverse of a quaternion.
[[nodiscard]] inline Quaternion inverse(const Quaternion& q) noexcept {
    return q.inverse();
}

/// @brief Normalized quaternion.
[[nodiscard]] inline Quaternion normalize(const Quaternion& q) noexcept {
    return q.normalized();
}

/// @brief Dot product of two quaternions.
[[nodiscard]] constexpr float dot(const Quaternion& a, const Quaternion& b) noexcept {
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z() + a.w() * b.w();
}

/// @brief Spherical linear interpolation.
[[nodiscard]] inline Quaternion slerp(const Quaternion& a, const Quaternion& b, float t) noexcept {
    return a.slerp(b, t);
}

/// @brief Returns the identity quaternion (no rotation).
[[nodiscard]] constexpr Quaternion quaternion_identity() noexcept {
    return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
}

} // namespace fc

/// @}
