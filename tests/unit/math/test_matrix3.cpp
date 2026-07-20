/// @file test_matrix3.cpp
/// @brief Unit tests for fc::Matrix3.

#include <fc/math/matrices/matrix3.hpp>

#include <cassert>
#include <cmath>
#include <cstdio>

static constexpr float kEps = 1e-4f;

static bool approx_eq(float a, float b, float eps = kEps) {
    return std::fabs(a - b) < eps;
}

void test_identity() {
    fc::Matrix3 m = fc::Matrix3::identity();
    assert(m(0, 0) == 1.0f);
    assert(m(1, 1) == 1.0f);
    assert(m(2, 2) == 1.0f);
    assert(m(0, 1) == 0.0f);
    assert(m(1, 0) == 0.0f);
    assert(m(2, 0) == 0.0f);
    std::printf("PASS: Matrix3 identity\n");
}

void test_constructor() {
    fc::Matrix3 m(
        1.0f, 2.0f, 3.0f,
        4.0f, 5.0f, 6.0f,
        7.0f, 8.0f, 9.0f
    );
    assert(m(0, 0) == 1.0f);
    assert(m(0, 1) == 2.0f);
    assert(m(0, 2) == 3.0f);
    assert(m(1, 0) == 4.0f);
    assert(m(1, 1) == 5.0f);
    assert(m(1, 2) == 6.0f);
    assert(m(2, 0) == 7.0f);
    assert(m(2, 1) == 8.0f);
    assert(m(2, 2) == 9.0f);
    std::printf("PASS: Matrix3 constructor\n");
}

void test_element_access() {
    fc::Matrix3 m = fc::Matrix3::identity();
    m(1, 2) = 5.0f;
    assert(m(1, 2) == 5.0f);
    std::printf("PASS: Matrix3 element access\n");
}

void test_matrix_multiply() {
    fc::Matrix3 a = fc::Matrix3::identity();
    fc::Matrix3 b = fc::Matrix3::identity();
    fc::Matrix3 c = a * b;
    assert(c == fc::Matrix3::identity());

    fc::Matrix3 s = fc::Matrix3::scale(2.0f);
    fc::Matrix3 product = s * s;
    assert(approx_eq(product(0, 0), 4.0f));
    assert(approx_eq(product(1, 1), 4.0f));
    assert(approx_eq(product(2, 2), 4.0f));
    std::printf("PASS: Matrix3 matrix multiply\n");
}

void test_vector_multiply() {
    fc::Matrix3 m = fc::Matrix3::identity();
    fc::Vector3 v(1.0f, 2.0f, 3.0f);
    fc::Vector3 r = m * v;
    assert(r.x() == 1.0f && r.y() == 2.0f && r.z() == 3.0f);

    fc::Matrix3 s = fc::Matrix3::scale(2.0f);
    fc::Vector3 r2 = s * v;
    assert(r2.x() == 2.0f && r2.y() == 4.0f && r2.z() == 6.0f);
    std::printf("PASS: Matrix3 vector multiply\n");
}

void test_scalar_multiply() {
    fc::Matrix3 m = fc::Matrix3::identity();
    fc::Matrix3 r = m * 3.0f;
    assert(approx_eq(r(0, 0), 3.0f));
    assert(approx_eq(r(1, 1), 3.0f));
    assert(approx_eq(r(2, 2), 3.0f));

    fc::Matrix3 r2 = 3.0f * m;
    assert(r2 == r);
    std::printf("PASS: Matrix3 scalar multiply\n");
}

void test_addition_subtraction() {
    fc::Matrix3 a = fc::Matrix3::identity();
    fc::Matrix3 b = fc::Matrix3::identity();
    fc::Matrix3 sum = a + b;
    assert(approx_eq(sum(0, 0), 2.0f));
    assert(approx_eq(sum(1, 1), 2.0f));
    assert(approx_eq(sum(2, 2), 2.0f));

    fc::Matrix3 diff = a - b;
    for (fc::usize i = 0; i < 3; ++i) {
        for (fc::usize j = 0; j < 3; ++j) {
            assert(approx_eq(diff(i, j), 0.0f));
        }
    }
    std::printf("PASS: Matrix3 addition/subtraction\n");
}

void test_transpose() {
    fc::Matrix3 m(
        1.0f, 2.0f, 3.0f,
        4.0f, 5.0f, 6.0f,
        7.0f, 8.0f, 9.0f
    );
    fc::Matrix3 t = m.transposed();
    assert(t(0, 0) == 1.0f);
    assert(t(0, 1) == 4.0f);
    assert(t(0, 2) == 7.0f);
    assert(t(1, 0) == 2.0f);
    assert(t(1, 1) == 5.0f);
    assert(t(1, 2) == 8.0f);
    assert(t(2, 0) == 3.0f);
    assert(t(2, 1) == 6.0f);
    assert(t(2, 2) == 9.0f);
    std::printf("PASS: Matrix3 transpose\n");
}

void test_determinant() {
    fc::Matrix3 identity = fc::Matrix3::identity();
    assert(identity.determinant() == 1.0f);

    fc::Matrix3 s = fc::Matrix3::scale(2.0f);
    assert(approx_eq(s.determinant(), 8.0f));

    fc::Matrix3 m(
        2.0f, 0.0f, 0.0f,
        0.0f, 3.0f, 0.0f,
        0.0f, 0.0f, 4.0f
    );
    assert(approx_eq(m.determinant(), 24.0f));
    std::printf("PASS: Matrix3 determinant\n");
}

void test_inverse() {
    fc::Matrix3 s = fc::Matrix3::scale(2.0f);
    fc::Matrix3 inv_s = s.inverse();
    fc::Matrix3 product = s * inv_s;
    assert(product == fc::Matrix3::identity());

    fc::Matrix3 rx = fc::Matrix3::rotation_x(0.5f);
    fc::Matrix3 rx_inv = rx.inverse();
    fc::Matrix3 product2 = rx * rx_inv;
    assert(product2 == fc::Matrix3::identity());
    std::printf("PASS: Matrix3 inverse\n");
}

void test_rotation_x() {
    fc::Matrix3 m = fc::Matrix3::rotation_x(0.0f);
    assert(m == fc::Matrix3::identity());
    std::printf("PASS: Matrix3 rotation_x\n");
}

void test_rotation_y() {
    fc::Matrix3 m = fc::Matrix3::rotation_y(0.0f);
    assert(m == fc::Matrix3::identity());
    std::printf("PASS: Matrix3 rotation_y\n");
}

void test_rotation_z() {
    fc::Matrix3 m = fc::Matrix3::rotation_z(0.0f);
    assert(m == fc::Matrix3::identity());
    std::printf("PASS: Matrix3 rotation_z\n");
}

void test_scale() {
    fc::Matrix3 m = fc::Matrix3::scale(3.0f);
    assert(approx_eq(m(0, 0), 3.0f));
    assert(approx_eq(m(1, 1), 3.0f));
    assert(approx_eq(m(2, 2), 3.0f));
    assert(m(0, 1) == 0.0f);

    fc::Matrix3 ns = fc::Matrix3::scale(1.0f, 2.0f, 3.0f);
    assert(approx_eq(ns(0, 0), 1.0f));
    assert(approx_eq(ns(1, 1), 2.0f));
    assert(approx_eq(ns(2, 2), 3.0f));
    std::printf("PASS: Matrix3 scale\n");
}

void test_comparison() {
    fc::Matrix3 a = fc::Matrix3::identity();
    fc::Matrix3 b = fc::Matrix3::identity();
    fc::Matrix3 c = fc::Matrix3::scale(2.0f);
    assert(a == b);
    assert(a != c);
    std::printf("PASS: Matrix3 comparison\n");
}

void test_free_functions() {
    fc::Matrix3 m = fc::Matrix3::identity();
    assert(fc::transpose(m) == m);
    assert(fc::determinant(m) == 1.0f);
    assert(fc::inverse(m) == m);

    fc::Matrix3 s = 2.0f * m;
    assert(approx_eq(s(0, 0), 2.0f));
    std::printf("PASS: Matrix3 free functions\n");
}

int main() {
    std::printf("Running Matrix3 tests...\n");

    test_identity();
    test_constructor();
    test_element_access();
    test_matrix_multiply();
    test_vector_multiply();
    test_scalar_multiply();
    test_addition_subtraction();
    test_transpose();
    test_determinant();
    test_inverse();
    test_rotation_x();
    test_rotation_y();
    test_rotation_z();
    test_scale();
    test_comparison();
    test_free_functions();

    std::printf("All Matrix3 tests passed.\n");
    return 0;
}
