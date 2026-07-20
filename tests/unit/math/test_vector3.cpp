/// @file test_vector3.cpp
/// @brief Unit tests for fc::Vector3.

#include <fc/math/vectors/vector3.hpp>

#include <cassert>
#include <cmath>
#include <cstdio>

static constexpr float kEps = 1e-5f;

static bool approx_eq(float a, float b, float eps = kEps) {
    return std::fabs(a - b) < eps;
}

[[maybe_unused]] static bool approx_eq(const fc::Vector3& a, const fc::Vector3& b, float eps = kEps) {
    return approx_eq(a.x(), b.x(), eps) && approx_eq(a.y(), b.y(), eps) && approx_eq(a.z(), b.z(), eps);
}

void test_default_constructor() {
    fc::Vector3 v;
    assert(v.x() == 0.0f);
    assert(v.y() == 0.0f);
    assert(v.z() == 0.0f);
    std::printf("PASS: Vector3 default constructor\n");
}

void test_component_constructor() {
    fc::Vector3 v(1.0f, 2.0f, 3.0f);
    assert(v.x() == 1.0f);
    assert(v.y() == 2.0f);
    assert(v.z() == 3.0f);
    std::printf("PASS: Vector3 component constructor\n");
}

void test_scalar_constructor() {
    fc::Vector3 v(5.0f);
    assert(v.x() == 5.0f);
    assert(v.y() == 5.0f);
    assert(v.z() == 5.0f);
    std::printf("PASS: Vector3 scalar constructor\n");
}

void test_element_access() {
    fc::Vector3 v(1.0f, 2.0f, 3.0f);
    assert(v[0] == 1.0f);
    assert(v[1] == 2.0f);
    assert(v[2] == 3.0f);
    v[1] = 10.0f;
    assert(v.y() == 10.0f);
    std::printf("PASS: Vector3 element access\n");
}

void test_addition() {
    fc::Vector3 a(1.0f, 2.0f, 3.0f);
    fc::Vector3 b(4.0f, 5.0f, 6.0f);
    [[maybe_unused]] fc::Vector3 c = a + b;
    assert(c.x() == 5.0f);
    assert(c.y() == 7.0f);
    assert(c.z() == 9.0f);
    std::printf("PASS: Vector3 addition\n");
}

void test_subtraction() {
    fc::Vector3 a(5.0f, 6.0f, 7.0f);
    fc::Vector3 b(2.0f, 3.0f, 4.0f);
    [[maybe_unused]] fc::Vector3 c = a - b;
    assert(c.x() == 3.0f);
    assert(c.y() == 3.0f);
    assert(c.z() == 3.0f);
    std::printf("PASS: Vector3 subtraction\n");
}

void test_scalar_multiply() {
    fc::Vector3 v(2.0f, 3.0f, 4.0f);
    [[maybe_unused]] fc::Vector3 r = v * 3.0f;
    assert(r.x() == 6.0f);
    assert(r.y() == 9.0f);
    assert(r.z() == 12.0f);

    [[maybe_unused]] fc::Vector3 r2 = 3.0f * v;
    assert(r2.x() == 6.0f);
    assert(r2.y() == 9.0f);
    assert(r2.z() == 12.0f);
    std::printf("PASS: Vector3 scalar multiply\n");
}

void test_scalar_divide() {
    fc::Vector3 v(6.0f, 9.0f, 12.0f);
    [[maybe_unused]] fc::Vector3 r = v / 3.0f;
    assert(r.x() == 2.0f);
    assert(r.y() == 3.0f);
    assert(r.z() == 4.0f);
    std::printf("PASS: Vector3 scalar divide\n");
}

void test_component_multiply() {
    fc::Vector3 a(2.0f, 3.0f, 4.0f);
    fc::Vector3 b(5.0f, 6.0f, 7.0f);
    [[maybe_unused]] fc::Vector3 r = a * b;
    assert(r.x() == 10.0f);
    assert(r.y() == 18.0f);
    assert(r.z() == 28.0f);
    std::printf("PASS: Vector3 component multiply\n");
}

void test_unary_negate() {
    fc::Vector3 v(1.0f, -2.0f, 3.0f);
    [[maybe_unused]] fc::Vector3 r = -v;
    assert(r.x() == -1.0f);
    assert(r.y() == 2.0f);
    assert(r.z() == -3.0f);
    std::printf("PASS: Vector3 unary negate\n");
}

void test_compound_assignment() {
    fc::Vector3 v(1.0f, 2.0f, 3.0f);
    v += fc::Vector3(4.0f, 5.0f, 6.0f);
    assert(v.x() == 5.0f && v.y() == 7.0f && v.z() == 9.0f);

    v -= fc::Vector3(1.0f, 2.0f, 3.0f);
    assert(v.x() == 4.0f && v.y() == 5.0f && v.z() == 6.0f);

    v *= 2.0f;
    assert(v.x() == 8.0f && v.y() == 10.0f && v.z() == 12.0f);

    v /= 2.0f;
    assert(v.x() == 4.0f && v.y() == 5.0f && v.z() == 6.0f);

    std::printf("PASS: Vector3 compound assignment\n");
}

void test_dot_product() {
    fc::Vector3 a(1.0f, 2.0f, 3.0f);
    fc::Vector3 b(4.0f, 5.0f, 6.0f);
    [[maybe_unused]] float d = a.dot(b);
    assert(d == 32.0f);

    [[maybe_unused]] float d2 = fc::dot(a, b);
    assert(d2 == 32.0f);
    std::printf("PASS: Vector3 dot product\n");
}

void test_cross_product() {
    fc::Vector3 x(1.0f, 0.0f, 0.0f);
    fc::Vector3 y(0.0f, 1.0f, 0.0f);
    [[maybe_unused]] fc::Vector3 z = x.cross(y);
    assert(z.x() == 0.0f && z.y() == 0.0f && z.z() == 1.0f);

    [[maybe_unused]] fc::Vector3 z2 = y.cross(x);
    assert(z2.x() == 0.0f && z2.y() == 0.0f && z2.z() == -1.0f);

    [[maybe_unused]] fc::Vector3 z3 = fc::cross(x, y);
    assert(z3.z() == 1.0f);
    std::printf("PASS: Vector3 cross product\n");
}

void test_length() {
    fc::Vector3 v(2.0f, 3.0f, 6.0f);
    assert(v.length_squared() == 49.0f);
    assert(v.length() == 7.0f);
    std::printf("PASS: Vector3 length\n");
}

void test_normalize() {
    fc::Vector3 v(0.0f, 3.0f, 4.0f);
    [[maybe_unused]] fc::Vector3 n = v.normalized();
    assert(approx_eq(n.length(), 1.0f));
    assert(approx_eq(n.x(), 0.0f));
    assert(approx_eq(n.y(), 0.6f));
    assert(approx_eq(n.z(), 0.8f));
    std::printf("PASS: Vector3 normalize\n");
}

void test_lerp() {
    fc::Vector3 a(0.0f, 0.0f, 0.0f);
    fc::Vector3 b(10.0f, 20.0f, 30.0f);
    [[maybe_unused]] fc::Vector3 mid = a.lerp(b, 0.5f);
    assert(approx_eq(mid.x(), 5.0f));
    assert(approx_eq(mid.y(), 10.0f));
    assert(approx_eq(mid.z(), 15.0f));
    std::printf("PASS: Vector3 lerp\n");
}

void test_comparison() {
    fc::Vector3 a(1.0f, 2.0f, 3.0f);
    fc::Vector3 b(1.0f, 2.0f, 3.0f);
    fc::Vector3 c(4.0f, 5.0f, 6.0f);
    assert(a == b);
    assert(a != c);
    std::printf("PASS: Vector3 comparison\n");
}

void test_free_functions() {
    fc::Vector3 a(0.0f, 0.0f, 0.0f);
    fc::Vector3 b(1.0f, 2.0f, 2.0f);
    assert(fc::distance(a, b) == 3.0f);
    assert(fc::distance_squared(a, b) == 9.0f);

    [[maybe_unused]] fc::Vector3 z = fc::vector3_zero();
    assert(z.x() == 0.0f && z.y() == 0.0f && z.z() == 0.0f);

    [[maybe_unused]] fc::Vector3 ux = fc::vector3_unit_x();
    assert(ux.x() == 1.0f && ux.y() == 0.0f && ux.z() == 0.0f);

    [[maybe_unused]] fc::Vector3 uy = fc::vector3_unit_y();
    assert(uy.x() == 0.0f && uy.y() == 1.0f && uy.z() == 0.0f);

    [[maybe_unused]] fc::Vector3 uz = fc::vector3_unit_z();
    assert(uz.x() == 0.0f && uz.y() == 0.0f && uz.z() == 1.0f);

    std::printf("PASS: Vector3 free functions\n");
}

void test_data_access() {
    fc::Vector3 v(1.0f, 2.0f, 3.0f);
    [[maybe_unused]] const float* d = v.data();
    assert(d[0] == 1.0f);
    assert(d[1] == 2.0f);
    assert(d[2] == 3.0f);
    std::printf("PASS: Vector3 data access\n");
}

int main() {
    std::printf("Running Vector3 tests...\n");

    test_default_constructor();
    test_component_constructor();
    test_scalar_constructor();
    test_element_access();
    test_addition();
    test_subtraction();
    test_scalar_multiply();
    test_scalar_divide();
    test_component_multiply();
    test_unary_negate();
    test_compound_assignment();
    test_dot_product();
    test_cross_product();
    test_length();
    test_normalize();
    test_lerp();
    test_comparison();
    test_free_functions();
    test_data_access();

    std::printf("All Vector3 tests passed.\n");
    return 0;
}
