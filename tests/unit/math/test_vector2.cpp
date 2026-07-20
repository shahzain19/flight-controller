/// @file test_vector2.cpp
/// @brief Unit tests for fc::Vector2.

#include <fc/math/vectors/vector2.hpp>

#include <cassert>
#include <cmath>
#include <cstdio>

static constexpr float kEps = 1e-5f;

static bool approx_eq(float a, float b, float eps = kEps) {
    return std::fabs(a - b) < eps;
}

[[maybe_unused]] static bool approx_eq(const fc::Vector2& a, const fc::Vector2& b, float eps = kEps) {
    return approx_eq(a.x(), b.x(), eps) && approx_eq(a.y(), b.y(), eps);
}

void test_default_constructor() {
    fc::Vector2 v;
    assert(v.x() == 0.0f);
    assert(v.y() == 0.0f);
    std::printf("PASS: Vector2 default constructor\n");
}

void test_component_constructor() {
    fc::Vector2 v(3.0f, 4.0f);
    assert(v.x() == 3.0f);
    assert(v.y() == 4.0f);
    std::printf("PASS: Vector2 component constructor\n");
}

void test_scalar_constructor() {
    fc::Vector2 v(5.0f);
    assert(v.x() == 5.0f);
    assert(v.y() == 5.0f);
    std::printf("PASS: Vector2 scalar constructor\n");
}

void test_element_access() {
    fc::Vector2 v(1.0f, 2.0f);
    assert(v[0] == 1.0f);
    assert(v[1] == 2.0f);
    v[0] = 10.0f;
    assert(v.x() == 10.0f);
    std::printf("PASS: Vector2 element access\n");
}

void test_addition() {
    fc::Vector2 a(1.0f, 2.0f);
    fc::Vector2 b(3.0f, 4.0f);
    [[maybe_unused]] fc::Vector2 c = a + b;
    assert(c.x() == 4.0f);
    assert(c.y() == 6.0f);
    std::printf("PASS: Vector2 addition\n");
}

void test_subtraction() {
    fc::Vector2 a(5.0f, 6.0f);
    fc::Vector2 b(2.0f, 3.0f);
    [[maybe_unused]] fc::Vector2 c = a - b;
    assert(c.x() == 3.0f);
    assert(c.y() == 3.0f);
    std::printf("PASS: Vector2 subtraction\n");
}

void test_scalar_multiply() {
    fc::Vector2 v(2.0f, 3.0f);
    [[maybe_unused]] fc::Vector2 r = v * 4.0f;
    assert(r.x() == 8.0f);
    assert(r.y() == 12.0f);

    [[maybe_unused]] fc::Vector2 r2 = 4.0f * v;
    assert(r2.x() == 8.0f);
    assert(r2.y() == 12.0f);
    std::printf("PASS: Vector2 scalar multiply\n");
}

void test_scalar_divide() {
    fc::Vector2 v(8.0f, 12.0f);
    [[maybe_unused]] fc::Vector2 r = v / 4.0f;
    assert(r.x() == 2.0f);
    assert(r.y() == 3.0f);
    std::printf("PASS: Vector2 scalar divide\n");
}

void test_component_multiply() {
    fc::Vector2 a(2.0f, 3.0f);
    fc::Vector2 b(4.0f, 5.0f);
    [[maybe_unused]] fc::Vector2 r = a * b;
    assert(r.x() == 8.0f);
    assert(r.y() == 15.0f);
    std::printf("PASS: Vector2 component multiply\n");
}

void test_unary_negate() {
    fc::Vector2 v(1.0f, -2.0f);
    [[maybe_unused]] fc::Vector2 r = -v;
    assert(r.x() == -1.0f);
    assert(r.y() == 2.0f);
    std::printf("PASS: Vector2 unary negate\n");
}

void test_compound_assignment() {
    fc::Vector2 v(1.0f, 2.0f);
    v += fc::Vector2(3.0f, 4.0f);
    assert(v.x() == 4.0f);
    assert(v.y() == 6.0f);

    v -= fc::Vector2(1.0f, 2.0f);
    assert(v.x() == 3.0f);
    assert(v.y() == 4.0f);

    v *= 2.0f;
    assert(v.x() == 6.0f);
    assert(v.y() == 8.0f);

    v /= 2.0f;
    assert(v.x() == 3.0f);
    assert(v.y() == 4.0f);

    std::printf("PASS: Vector2 compound assignment\n");
}

void test_dot_product() {
    fc::Vector2 a(1.0f, 2.0f);
    fc::Vector2 b(3.0f, 4.0f);
    [[maybe_unused]] float d = a.dot(b);
    assert(d == 11.0f);

    [[maybe_unused]] float d2 = fc::dot(a, b);
    assert(d2 == 11.0f);
    std::printf("PASS: Vector2 dot product\n");
}

void test_length() {
    fc::Vector2 v(3.0f, 4.0f);
    assert(v.length_squared() == 25.0f);
    assert(v.length() == 5.0f);
    std::printf("PASS: Vector2 length\n");
}

void test_normalize() {
    fc::Vector2 v(3.0f, 4.0f);
    [[maybe_unused]] fc::Vector2 n = v.normalized();
    assert(approx_eq(n.length(), 1.0f));
    assert(approx_eq(n.x(), 0.6f));
    assert(approx_eq(n.y(), 0.8f));
    std::printf("PASS: Vector2 normalize\n");
}

void test_lerp() {
    fc::Vector2 a(0.0f, 0.0f);
    fc::Vector2 b(10.0f, 20.0f);
    [[maybe_unused]] fc::Vector2 mid = a.lerp(b, 0.5f);
    assert(approx_eq(mid.x(), 5.0f));
    assert(approx_eq(mid.y(), 10.0f));
    std::printf("PASS: Vector2 lerp\n");
}

void test_comparison() {
    fc::Vector2 a(1.0f, 2.0f);
    fc::Vector2 b(1.0f, 2.0f);
    fc::Vector2 c(3.0f, 4.0f);
    assert(a == b);
    assert(a != c);
    std::printf("PASS: Vector2 comparison\n");
}

void test_free_functions() {
    fc::Vector2 a(0.0f, 0.0f);
    fc::Vector2 b(3.0f, 4.0f);
    assert(fc::distance(a, b) == 5.0f);
    assert(fc::distance_squared(a, b) == 25.0f);

    fc::Vector2 p(1.0f, 2.0f);
    [[maybe_unused]] fc::Vector2 perp = fc::perpendicular(p);
    assert(perp.x() == -2.0f);
    assert(perp.y() == 1.0f);

    [[maybe_unused]] fc::Vector2 z = fc::vector2_zero();
    assert(z.x() == 0.0f && z.y() == 0.0f);

    [[maybe_unused]] fc::Vector2 ux = fc::vector2_unit_x();
    assert(ux.x() == 1.0f && ux.y() == 0.0f);

    [[maybe_unused]] fc::Vector2 uy = fc::vector2_unit_y();
    assert(uy.x() == 0.0f && uy.y() == 1.0f);

    std::printf("PASS: Vector2 free functions\n");
}

void test_data_access() {
    fc::Vector2 v(1.0f, 2.0f);
    [[maybe_unused]] const float* d = v.data();
    assert(d[0] == 1.0f);
    assert(d[1] == 2.0f);
    std::printf("PASS: Vector2 data access\n");
}

int main() {
    std::printf("Running Vector2 tests...\n");

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
    test_length();
    test_normalize();
    test_lerp();
    test_comparison();
    test_free_functions();
    test_data_access();

    std::printf("All Vector2 tests passed.\n");
    return 0;
}
