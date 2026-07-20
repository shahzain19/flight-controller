/// @file matrix3.hpp
/// @brief 3x3 matrix class for the flight computer.
///
/// A 3x3 matrix stored in column-major order for OpenGL/graphics convention.
/// Designed for SIMD auto-vectorization with aligned storage.
///
/// Storage Layout (column-major):
///   | m00 m01 m02 |   data[0] data[3] data[6]
///   | m10 m11 m12 | = data[1] data[4] data[7]
///   | m20 m21 m22 |   data[2] data[5] data[8]
///
/// Design:
///   - Header-first, fully constexpr
///   - alignas(16) for SIMD register loading
///   - Column-major storage for GPU/graphics compatibility
///   - No dynamic allocation
///   - No exceptions
///
/// Numerical Assumptions:
///   - IEEE 754 single precision (~7 significant digits)
///   - Determinant near zero means matrix is singular (non-invertible)
///   - inverse() assumes non-singular matrix
///
/// @defgroup math_matrix3 Matrix3
/// @{

#pragma once

#include "../../core/types/types.hpp"
#include "../../core/compiler.hpp"
#include "../vectors/vector3.hpp"

#include <cmath>

namespace fc {

/// @brief 3x3 matrix stored in column-major order.
///
/// Storage: 36 bytes (9 floats), alignas(16).
/// Time complexity: O(1) for element access, O(n^3) for multiply (n=3, so O(1)).
class Matrix3 {
public:
    // ========================================================================
    // Constructors
    // ========================================================================

    /// @brief Default constructor. Initializes to zero matrix.
    constexpr Matrix3() noexcept = default;

    /// @brief Constructs a Matrix3 from 9 floats (column-major order).
    constexpr Matrix3(
        float m00, float m01, float m02,
        float m10, float m11, float m12,
        float m20, float m21, float m22
    ) noexcept : data_{m00, m10, m20, m01, m11, m21, m02, m12, m22} {}

    /// @brief Constructs a Matrix3 from a column-major array.
    constexpr explicit Matrix3(const float (&arr)[9]) noexcept {
        for (usize i = 0; i < 9; ++i) data_[i] = arr[i];
    }

    // ========================================================================
    // Factory Methods
    // ========================================================================

    /// @brief Returns the identity matrix.
    [[nodiscard]] static constexpr Matrix3 identity() noexcept {
        return Matrix3(
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
        );
    }

    /// @brief Returns a matrix scaled uniformly.
    [[nodiscard]] static constexpr Matrix3 scale(float s) noexcept {
        return Matrix3(
            s,    0.0f, 0.0f,
            0.0f, s,    0.0f,
            0.0f, 0.0f, s
        );
    }

    /// @brief Returns a matrix scaled non-uniformly.
    [[nodiscard]] static constexpr Matrix3 scale(float sx, float sy, float sz) noexcept {
        return Matrix3(
            sx,   0.0f, 0.0f,
            0.0f, sy,   0.0f,
            0.0f, 0.0f, sz
        );
    }

    /// @brief Returns a rotation matrix around the X axis.
    ///
    /// @param angle_rad Rotation angle in radians.
    [[nodiscard]] static Matrix3 rotation_x(float angle_rad) noexcept {
        float c = std::cos(angle_rad);
        float s = std::sin(angle_rad);
        return Matrix3(
            1.0f, 0.0f, 0.0f,
            0.0f, c,    -s,
            0.0f, s,    c
        );
    }

    /// @brief Returns a rotation matrix around the Y axis.
    ///
    /// @param angle_rad Rotation angle in radians.
    [[nodiscard]] static Matrix3 rotation_y(float angle_rad) noexcept {
        float c = std::cos(angle_rad);
        float s = std::sin(angle_rad);
        return Matrix3(
            c,    0.0f, s,
            0.0f, 1.0f, 0.0f,
            -s,   0.0f, c
        );
    }

    /// @brief Returns a rotation matrix around the Z axis.
    ///
    /// @param angle_rad Rotation angle in radians.
    [[nodiscard]] static Matrix3 rotation_z(float angle_rad) noexcept {
        float c = std::cos(angle_rad);
        float s = std::sin(angle_rad);
        return Matrix3(
            c,    -s,   0.0f,
            s,    c,    0.0f,
            0.0f, 0.0f, 1.0f
        );
    }

    // ========================================================================
    // Element Access
    // ========================================================================

    /// @brief Access element by row and column (const).
    [[nodiscard]] constexpr float operator()(usize row, usize col) const noexcept {
        return data_[col * 3 + row];
    }

    /// @brief Access element by row and column (mutable).
    [[nodiscard]] constexpr float& operator()(usize row, usize col) noexcept {
        return data_[col * 3 + row];
    }

    // ========================================================================
    // Arithmetic Operators
    // ========================================================================

    /// @brief Matrix-matrix multiplication.
    ///
    /// Complexity: O(n^3) = O(27) = O(1)
    /// WCET: < 200 ns
    [[nodiscard]] constexpr Matrix3 operator*(const Matrix3& other) const noexcept {
        Matrix3 result;
        for (usize col = 0; col < 3; ++col) {
            for (usize row = 0; row < 3; ++row) {
                float sum = 0.0f;
                for (usize k = 0; k < 3; ++k) {
                    sum += data_[k * 3 + row] * other.data_[col * 3 + k];
                }
                result.data_[col * 3 + row] = sum;
            }
        }
        return result;
    }

    /// @brief Matrix-vector multiplication.
    ///
    /// Complexity: O(n^2) = O(9) = O(1)
    /// WCET: < 100 ns
    [[nodiscard]] constexpr Vector3 operator*(const Vector3& vec) const noexcept {
        return Vector3(
            data_[0] * vec.x() + data_[3] * vec.y() + data_[6] * vec.z(),
            data_[1] * vec.x() + data_[4] * vec.y() + data_[7] * vec.z(),
            data_[2] * vec.x() + data_[5] * vec.y() + data_[8] * vec.z()
        );
    }

    /// @brief Scalar multiplication.
    [[nodiscard]] constexpr Matrix3 operator*(float scalar) const noexcept {
        Matrix3 result;
        for (usize i = 0; i < 9; ++i) {
            result.data_[i] = data_[i] * scalar;
        }
        return result;
    }

    /// @brief Matrix addition.
    [[nodiscard]] constexpr Matrix3 operator+(const Matrix3& other) const noexcept {
        Matrix3 result;
        for (usize i = 0; i < 9; ++i) {
            result.data_[i] = data_[i] + other.data_[i];
        }
        return result;
    }

    /// @brief Matrix subtraction.
    [[nodiscard]] constexpr Matrix3 operator-(const Matrix3& other) const noexcept {
        Matrix3 result;
        for (usize i = 0; i < 9; ++i) {
            result.data_[i] = data_[i] - other.data_[i];
        }
        return result;
    }

    /// @brief Unary negation.
    [[nodiscard]] constexpr Matrix3 operator-() const noexcept {
        Matrix3 result;
        for (usize i = 0; i < 9; ++i) {
            result.data_[i] = -data_[i];
        }
        return result;
    }

    // ========================================================================
    // Matrix Operations
    // ========================================================================

    /// @brief Returns the transpose.
    ///
    /// Complexity: O(n^2) = O(9) = O(1)
    /// WCET: < 50 ns
    [[nodiscard]] constexpr Matrix3 transposed() const noexcept {
        return Matrix3(
            data_[0], data_[1], data_[2],
            data_[3], data_[4], data_[5],
            data_[6], data_[7], data_[8]
        );
    }

    /// @brief Returns the determinant.
    ///
    /// Complexity: O(1) (12 multiplications, 5 additions)
    /// WCET: < 50 ns
    [[nodiscard]] constexpr float determinant() const noexcept {
        return data_[0] * (data_[4] * data_[8] - data_[5] * data_[7])
             - data_[3] * (data_[1] * data_[8] - data_[2] * data_[7])
             + data_[6] * (data_[1] * data_[5] - data_[2] * data_[4]);
    }

    /// @brief Returns the inverse.
    ///
    /// @note Assumes non-singular matrix (determinant != 0).
    /// Behavior is undefined if determinant is near zero.
    ///
    /// Complexity: O(1)
    /// WCET: < 150 ns
    [[nodiscard]] Matrix3 inverse() const noexcept {
        float det = determinant();
        float inv_det = 1.0f / det;

        // Adjugate = transpose of cofactor matrix
        return Matrix3(
            (data_[4] * data_[8] - data_[7] * data_[5]) * inv_det,
            (data_[6] * data_[5] - data_[3] * data_[8]) * inv_det,
            (data_[3] * data_[7] - data_[6] * data_[4]) * inv_det,

            (data_[7] * data_[2] - data_[1] * data_[8]) * inv_det,
            (data_[0] * data_[8] - data_[6] * data_[2]) * inv_det,
            (data_[6] * data_[1] - data_[0] * data_[7]) * inv_det,

            (data_[1] * data_[5] - data_[4] * data_[2]) * inv_det,
            (data_[3] * data_[2] - data_[0] * data_[5]) * inv_det,
            (data_[0] * data_[4] - data_[3] * data_[1]) * inv_det
        );
    }

    // ========================================================================
    // Comparison
    // ========================================================================

    [[nodiscard]] constexpr bool operator==(const Matrix3& other) const noexcept {
        for (usize i = 0; i < 9; ++i) {
            if (data_[i] != other.data_[i]) return false;
        }
        return true;
    }

    [[nodiscard]] constexpr bool operator!=(const Matrix3& other) const noexcept {
        return !(*this == other);
    }

    // ========================================================================
    // Data Access
    // ========================================================================

    /// @brief Returns a pointer to the raw data (column-major).
    [[nodiscard]] constexpr float* data() noexcept { return data_; }

    /// @brief Returns a const pointer to the raw data (column-major).
    [[nodiscard]] constexpr const float* data() const noexcept { return data_; }

private:
    alignas(16) float data_[9] = {
        1.0f, 0.0f, 0.0f,  // column 0
        0.0f, 1.0f, 0.0f,  // column 1
        0.0f, 0.0f, 1.0f   // column 2
    };
};

// ============================================================================
// Free Functions
// ============================================================================

/// @brief Scalar * Matrix.
[[nodiscard]] constexpr Matrix3 operator*(float scalar, const Matrix3& mat) noexcept {
    return mat * scalar;
}

/// @brief Transpose of a matrix.
[[nodiscard]] constexpr Matrix3 transpose(const Matrix3& mat) noexcept {
    return mat.transposed();
}

/// @brief Determinant of a matrix.
[[nodiscard]] constexpr float determinant(const Matrix3& mat) noexcept {
    return mat.determinant();
}

/// @brief Inverse of a matrix.
[[nodiscard]] inline Matrix3 inverse(const Matrix3& mat) noexcept {
    return mat.inverse();
}

} // namespace fc

/// @}
