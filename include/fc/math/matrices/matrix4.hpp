/// @file matrix4.hpp
/// @brief 4x4 matrix class for the flight computer.
///
/// A 4x4 matrix stored in column-major order for OpenGL/graphics convention.
/// Designed for SIMD auto-vectorization with aligned storage.
///
/// Storage Layout (column-major):
///   | m00 m01 m02 m03 |   data[0]  data[4]  data[8]  data[12]
///   | m10 m11 m12 m13 | = data[1]  data[5]  data[9]  data[13]
///   | m20 m21 m22 m23 |   data[2]  data[6]  data[10] data[14]
///   | m30 m31 m32 m33 |   data[3]  data[7]  data[11] data[15]
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
/// @defgroup math_matrix4 Matrix4
/// @{

#pragma once

#include "../../core/types/types.hpp"
#include "../../core/compiler.hpp"
#include "../vectors/vector3.hpp"
#include "matrix3.hpp"

#include <cmath>

namespace fc {

/// @brief 4x4 matrix stored in column-major order.
///
/// Storage: 64 bytes (16 floats), alignas(16).
/// Time complexity: O(1) for element access, O(n^3) for multiply (n=4, so O(1)).
class Matrix4 {
public:
    // ========================================================================
    // Constructors
    // ========================================================================

    /// @brief Default constructor. Initializes to zero matrix.
    constexpr Matrix4() noexcept = default;

    /// @brief Constructs a Matrix4 from 16 floats (column-major order).
    constexpr Matrix4(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33
    ) noexcept : data_{
        m00, m10, m20, m30,
        m01, m11, m21, m31,
        m02, m12, m22, m32,
        m03, m13, m23, m33
    } {}

    /// @brief Constructs a Matrix4 from a column-major array.
    constexpr explicit Matrix4(const float (&arr)[16]) noexcept {
        for (usize i = 0; i < 16; ++i) data_[i] = arr[i];
    }

    // ========================================================================
    // Factory Methods
    // ========================================================================

    /// @brief Returns the identity matrix.
    [[nodiscard]] static constexpr Matrix4 identity() noexcept {
        return Matrix4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    /// @brief Returns a translation matrix.
    [[nodiscard]] static constexpr Matrix4 translation(float x, float y, float z) noexcept {
        return Matrix4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            x,    y,    z,    1.0f
        );
    }

    /// @brief Returns a translation matrix from a Vector3.
    [[nodiscard]] static constexpr Matrix4 translation(const Vector3& t) noexcept {
        return translation(t.x(), t.y(), t.z());
    }

    /// @brief Returns a uniform scale matrix.
    [[nodiscard]] static constexpr Matrix4 scale(float s) noexcept {
        return Matrix4(
            s,    0.0f, 0.0f, 0.0f,
            0.0f, s,    0.0f, 0.0f,
            0.0f, 0.0f, s,    0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    /// @brief Returns a non-uniform scale matrix.
    [[nodiscard]] static constexpr Matrix4 scale(float sx, float sy, float sz) noexcept {
        return Matrix4(
            sx,   0.0f, 0.0f, 0.0f,
            0.0f, sy,   0.0f, 0.0f,
            0.0f, 0.0f, sz,   0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    /// @brief Returns a rotation matrix around the X axis.
    [[nodiscard]] static Matrix4 rotation_x(float angle_rad) noexcept {
        float c = std::cos(angle_rad);
        float s = std::sin(angle_rad);
        return Matrix4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, c,    -s,   0.0f,
            0.0f, s,    c,    0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    /// @brief Returns a rotation matrix around the Y axis.
    [[nodiscard]] static Matrix4 rotation_y(float angle_rad) noexcept {
        float c = std::cos(angle_rad);
        float s = std::sin(angle_rad);
        return Matrix4(
            c,    0.0f, s,    0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -s,   0.0f, c,    0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    /// @brief Returns a rotation matrix around the Z axis.
    [[nodiscard]] static Matrix4 rotation_z(float angle_rad) noexcept {
        float c = std::cos(angle_rad);
        float s = std::sin(angle_rad);
        return Matrix4(
            c,    -s,   0.0f, 0.0f,
            s,    c,    0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    /// @brief Returns a look-at view matrix.
    ///
    /// @param eye Camera position.
    /// @param target Target position to look at.
    /// @param up Up vector (typically world up).
    [[nodiscard]] static Matrix4 look_at(const Vector3& eye, const Vector3& target, const Vector3& up) noexcept {
        Vector3 f = (target - eye).normalized();
        Vector3 s = f.cross(up).normalized();
        Vector3 u = s.cross(f);

        return Matrix4(
            s.x(),    u.x(),    -f.x(),   0.0f,
            s.y(),    u.y(),    -f.y(),   0.0f,
            s.z(),    u.z(),    -f.z(),   0.0f,
            -s.dot(eye), -u.dot(eye), f.dot(eye), 1.0f
        );
    }

    /// @brief Returns a perspective projection matrix.
    ///
    /// @param fov_y_rad Vertical field of view in radians.
    /// @param aspect Aspect ratio (width / height).
    /// @param near Near clipping plane distance (must be > 0).
    /// @param far Far clipping plane distance (must be > near).
    [[nodiscard]] static Matrix4 perspective(float fov_y_rad, float aspect, float near, float far) noexcept {
        float tan_half_fov = std::tan(fov_y_rad * 0.5f);
        float range_inv = 1.0f / (near - far);

        return Matrix4(
            1.0f / (aspect * tan_half_fov), 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f / tan_half_fov, 0.0f, 0.0f,
            0.0f, 0.0f, (near + far) * range_inv, -1.0f,
            0.0f, 0.0f, near * far * range_inv * 2.0f, 0.0f
        );
    }

    /// @brief Returns an orthographic projection matrix.
    [[nodiscard]] static constexpr Matrix4 ortho(float left, float right, float bottom, float top, float near, float far) noexcept {
        float rl_inv = 1.0f / (right - left);
        float tb_inv = 1.0f / (top - bottom);
        float fn_inv = 1.0f / (far - near);

        return Matrix4(
            2.0f * rl_inv, 0.0f, 0.0f, 0.0f,
            0.0f, 2.0f * tb_inv, 0.0f, 0.0f,
            0.0f, 0.0f, -2.0f * fn_inv, 0.0f,
            -(right + left) * rl_inv, -(top + bottom) * tb_inv, -(far + near) * fn_inv, 1.0f
        );
    }

    // ========================================================================
    // Element Access
    // ========================================================================

    /// @brief Access element by row and column (const).
    [[nodiscard]] constexpr float operator()(usize row, usize col) const noexcept {
        return data_[col * 4 + row];
    }

    /// @brief Access element by row and column (mutable).
    [[nodiscard]] constexpr float& operator()(usize row, usize col) noexcept {
        return data_[col * 4 + row];
    }

    // ========================================================================
    // Arithmetic Operators
    // ========================================================================

    /// @brief Matrix-matrix multiplication.
    ///
    /// Complexity: O(n^3) = O(64) = O(1)
    /// WCET: < 300 ns
    [[nodiscard]] constexpr Matrix4 operator*(const Matrix4& other) const noexcept {
        Matrix4 result;
        for (usize col = 0; col < 4; ++col) {
            for (usize row = 0; row < 4; ++row) {
                float sum = 0.0f;
                for (usize k = 0; k < 4; ++k) {
                    sum += data_[k * 4 + row] * other.data_[col * 4 + k];
                }
                result.data_[col * 4 + row] = sum;
            }
        }
        return result;
    }

    /// @brief Scalar multiplication.
    [[nodiscard]] constexpr Matrix4 operator*(float scalar) const noexcept {
        Matrix4 result;
        for (usize i = 0; i < 16; ++i) {
            result.data_[i] = data_[i] * scalar;
        }
        return result;
    }

    /// @brief Matrix addition.
    [[nodiscard]] constexpr Matrix4 operator+(const Matrix4& other) const noexcept {
        Matrix4 result;
        for (usize i = 0; i < 16; ++i) {
            result.data_[i] = data_[i] + other.data_[i];
        }
        return result;
    }

    /// @brief Matrix subtraction.
    [[nodiscard]] constexpr Matrix4 operator-(const Matrix4& other) const noexcept {
        Matrix4 result;
        for (usize i = 0; i < 16; ++i) {
            result.data_[i] = data_[i] - other.data_[i];
        }
        return result;
    }

    /// @brief Unary negation.
    [[nodiscard]] constexpr Matrix4 operator-() const noexcept {
        Matrix4 result;
        for (usize i = 0; i < 16; ++i) {
            result.data_[i] = -data_[i];
        }
        return result;
    }

    // ========================================================================
    // Matrix Operations
    // ========================================================================

    /// @brief Returns the transpose.
    ///
    /// Complexity: O(n^2) = O(16) = O(1)
    /// WCET: < 50 ns
    [[nodiscard]] constexpr Matrix4 transposed() const noexcept {
        return Matrix4(
            data_[0], data_[1], data_[2], data_[3],
            data_[4], data_[5], data_[6], data_[7],
            data_[8], data_[9], data_[10], data_[11],
            data_[12], data_[13], data_[14], data_[15]
        );
    }

    /// @brief Returns the determinant.
    ///
    /// Complexity: O(1)
    /// WCET: < 100 ns
    [[nodiscard]] constexpr float determinant() const noexcept {
        // Expand along the first row
        float a = data_[0], b = data_[4], c = data_[8], d = data_[12];

        float m1 = data_[5] * (data_[10] * data_[15] - data_[11] * data_[14])
                 - data_[9] * (data_[6] * data_[15] - data_[7] * data_[14])
                 + data_[13] * (data_[6] * data_[11] - data_[7] * data_[10]);

        float m2 = data_[1] * (data_[10] * data_[15] - data_[11] * data_[14])
                 - data_[9] * (data_[2] * data_[15] - data_[3] * data_[14])
                 + data_[13] * (data_[2] * data_[11] - data_[3] * data_[10]);

        float m3 = data_[1] * (data_[6] * data_[15] - data_[7] * data_[14])
                 - data_[5] * (data_[2] * data_[15] - data_[3] * data_[14])
                 + data_[13] * (data_[2] * data_[7] - data_[3] * data_[6]);

        float m4 = data_[1] * (data_[6] * data_[11] - data_[7] * data_[10])
                 - data_[5] * (data_[2] * data_[11] - data_[3] * data_[10])
                 + data_[9] * (data_[2] * data_[7] - data_[3] * data_[6]);

        return a * m1 - b * m2 + c * m3 - d * m4;
    }

    /// @brief Returns the upper-left 3x3 submatrix.
    [[nodiscard]] constexpr Matrix3 upper3x3() const noexcept {
        return Matrix3(
            data_[0], data_[1], data_[2],
            data_[4], data_[5], data_[6],
            data_[8], data_[9], data_[10]
        );
    }

    // ========================================================================
    // Comparison
    // ========================================================================

    [[nodiscard]] constexpr bool operator==(const Matrix4& other) const noexcept {
        for (usize i = 0; i < 16; ++i) {
            if (data_[i] != other.data_[i]) return false;
        }
        return true;
    }

    [[nodiscard]] constexpr bool operator!=(const Matrix4& other) const noexcept {
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
    alignas(16) float data_[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,  // column 0
        0.0f, 1.0f, 0.0f, 0.0f,  // column 1
        0.0f, 0.0f, 1.0f, 0.0f,  // column 2
        0.0f, 0.0f, 0.0f, 1.0f   // column 3
    };
};

// ============================================================================
// Free Functions
// ============================================================================

/// @brief Scalar * Matrix.
[[nodiscard]] constexpr Matrix4 operator*(float scalar, const Matrix4& mat) noexcept {
    return mat * scalar;
}

/// @brief Transpose of a matrix.
[[nodiscard]] constexpr Matrix4 transpose(const Matrix4& mat) noexcept {
    return mat.transposed();
}

/// @brief Determinant of a matrix.
[[nodiscard]] constexpr float determinant(const Matrix4& mat) noexcept {
    return mat.determinant();
}

} // namespace fc

/// @}
