/// @file test_matrix4.cpp
/// @brief Unit tests for fc::Matrix4.

#include <fc/math/matrices/matrix4.hpp>

#include <cassert>
#include <cmath>
#include <cstdio>

static constexpr float kEps = 1e-4f;

static bool approx_eq(float a, float b, float eps = kEps) {
    return std::fabs(a - b) < eps;
}

void test_identity() {
    fc::Matrix4 m = fc::Matrix4::identity();
    assert(m(0, 0) == 1.0f);
    assert(m(1, 1) == 1.0f);
    assert(m(2, 2) == 1.0f);
    assert(m(3, 3) == 1.0f);
    assert(m(0, 1) == 0.0f);
    assert(m(1, 0) == 0.0f);
    assert(m(3, 0) == 0.0f);
    std::printf("PASS: Matrix4 identity\n");
}

void test_constructor() {
    fc::Matrix4 m(
        1.0f,  2.0f,  3.0f,  4.0f,
        5.0f,  6.0f,  7.0f,  8.0f,
        9.0f,  10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    );
    assert(m(0, 0) == 1.0f);
    assert(m(0, 3) == 4.0f);
    assert(m(3, 0) == 13.0f);
    assert(m(3, 3) == 16.0f);
    std::printf("PASS: Matrix4 constructor\n");
}

void test_element_access() {
    fc::Matrix4 m = fc::Matrix4::identity();
    m(2, 3) = 99.0f;
    assert(m(2, 3) == 99.0f);
    std::printf("PASS: Matrix4 element access\n");
}

void test_matrix_multiply() {
    fc::Matrix4 a = fc::Matrix4::identity();
    fc::Matrix4 b = fc::Matrix4::identity();
    fc::Matrix4 c = a * b;
    assert(c == fc::Matrix4::identity());

    fc::Matrix4 s = fc::Matrix4::scale(2.0f);
    fc::Matrix4 product = s * s;
    assert(approx_eq(product(0, 0), 4.0f));
    assert(approx_eq(product(1, 1), 4.0f));
    assert(approx_eq(product(2, 2), 4.0f));
    assert(approx_eq(product(3, 3), 1.0f));
    std::printf("PASS: Matrix4 matrix multiply\n");
}

void test_scalar_multiply() {
    fc::Matrix4 m = fc::Matrix4::identity();
    fc::Matrix4 r = m * 3.0f;
    assert(approx_eq(r(0, 0), 3.0f));
    assert(approx_eq(r(1, 1), 3.0f));
    assert(approx_eq(r(2, 2), 3.0f));
    assert(approx_eq(r(3, 3), 3.0f));

    fc::Matrix4 r2 = 3.0f * m;
    assert(r2 == r);
    std::printf("PASS: Matrix4 scalar multiply\n");
}

void test_addition_subtraction() {
    fc::Matrix4 a = fc::Matrix4::identity();
    fc::Matrix4 b = fc::Matrix4::identity();
    fc::Matrix4 sum = a + b;
    assert(approx_eq(sum(0, 0), 2.0f));
    assert(approx_eq(sum(3, 3), 2.0f));

    fc::Matrix4 diff = a - b;
    for (fc::usize i = 0; i < 4; ++i) {
        for (fc::usize j = 0; j < 4; ++j) {
            assert(approx_eq(diff(i, j), 0.0f));
        }
    }
    std::printf("PASS: Matrix4 addition/subtraction\n");
}

void test_transpose() {
    fc::Matrix4 m(
        1.0f,  2.0f,  3.0f,  4.0f,
        5.0f,  6.0f,  7.0f,  8.0f,
        9.0f,  10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    );
    fc::Matrix4 t = m.transposed();
    assert(t(0, 1) == 5.0f);
    assert(t(1, 0) == 2.0f);
    assert(t(0, 3) == 13.0f);
    assert(t(3, 0) == 4.0f);
    std::printf("PASS: Matrix4 transpose\n");
}

void test_determinant() {
    fc::Matrix4 identity = fc::Matrix4::identity();
    assert(identity.determinant() == 1.0f);

    fc::Matrix4 s = fc::Matrix4::scale(2.0f);
    assert(approx_eq(s.determinant(), 8.0f));

    fc::Matrix4 t = fc::Matrix4::translation(5.0f, 10.0f, 15.0f);
    assert(approx_eq(t.determinant(), 1.0f));
    std::printf("PASS: Matrix4 determinant\n");
}

void test_translation() {
    fc::Matrix4 t = fc::Matrix4::translation(1.0f, 2.0f, 3.0f);
    assert(t(3, 0) == 1.0f);
    assert(t(3, 1) == 2.0f);
    assert(t(3, 2) == 3.0f);
    assert(t(3, 3) == 1.0f);
    assert(t(0, 0) == 1.0f);
    assert(t(1, 1) == 1.0f);
    assert(t(2, 2) == 1.0f);

    fc::Vector3 v(1.0f, 0.0f, 0.0f);
    fc::Matrix4 t2 = fc::Matrix4::translation(5.0f, 0.0f, 0.0f);
    (void)t2;
    (void)v;
    std::printf("PASS: Matrix4 translation\n");
}

void test_scale() {
    fc::Matrix4 m = fc::Matrix4::scale(3.0f);
    assert(approx_eq(m(0, 0), 3.0f));
    assert(approx_eq(m(1, 1), 3.0f));
    assert(approx_eq(m(2, 2), 3.0f));
    assert(approx_eq(m(3, 3), 1.0f));
    assert(m(0, 1) == 0.0f);

    fc::Matrix4 ns = fc::Matrix4::scale(1.0f, 2.0f, 3.0f);
    assert(approx_eq(ns(0, 0), 1.0f));
    assert(approx_eq(ns(1, 1), 2.0f));
    assert(approx_eq(ns(2, 2), 3.0f));
    std::printf("PASS: Matrix4 scale\n");
}

void test_rotation_x() {
    fc::Matrix4 m = fc::Matrix4::rotation_x(0.0f);
    assert(m == fc::Matrix4::identity());
    std::printf("PASS: Matrix4 rotation_x\n");
}

void test_rotation_y() {
    fc::Matrix4 m = fc::Matrix4::rotation_y(0.0f);
    assert(m == fc::Matrix4::identity());
    std::printf("PASS: Matrix4 rotation_y\n");
}

void test_rotation_z() {
    fc::Matrix4 m = fc::Matrix4::rotation_z(0.0f);
    assert(m == fc::Matrix4::identity());
    std::printf("PASS: Matrix4 rotation_z\n");
}

void test_upper3x3() {
    fc::Matrix4 m = fc::Matrix4::identity();
    fc::Matrix3 upper = m.upper3x3();
    assert(upper == fc::Matrix3::identity());

    fc::Matrix4 s = fc::Matrix4::scale(2.0f);
    fc::Matrix3 upper_s = s.upper3x3();
    assert(approx_eq(upper_s(0, 0), 2.0f));
    assert(approx_eq(upper_s(1, 1), 2.0f));
    assert(approx_eq(upper_s(2, 2), 2.0f));
    std::printf("PASS: Matrix4 upper3x3\n");
}

void test_comparison() {
    fc::Matrix4 a = fc::Matrix4::identity();
    fc::Matrix4 b = fc::Matrix4::identity();
    fc::Matrix4 c = fc::Matrix4::scale(2.0f);
    assert(a == b);
    assert(a != c);
    std::printf("PASS: Matrix4 comparison\n");
}

void test_free_functions() {
    fc::Matrix4 m = fc::Matrix4::identity();
    assert(fc::transpose(m) == m);
    assert(fc::determinant(m) == 1.0f);

    fc::Matrix4 s = 2.0f * m;
    assert(approx_eq(s(0, 0), 2.0f));
    std::printf("PASS: Matrix4 free functions\n");
}

int main() {
    std::printf("Running Matrix4 tests...\n");

    test_identity();
    test_constructor();
    test_element_access();
    test_matrix_multiply();
    test_scalar_multiply();
    test_addition_subtraction();
    test_transpose();
    test_determinant();
    test_translation();
    test_scale();
    test_rotation_x();
    test_rotation_y();
    test_rotation_z();
    test_upper3x3();
    test_comparison();
    test_free_functions();

    std::printf("All Matrix4 tests passed.\n");
    return 0;
}
