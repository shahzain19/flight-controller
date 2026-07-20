/// @file test_quaternion.cpp
/// @brief Unit tests for fc::Quaternion.

#include <fc/math/quaternions/quaternion.hpp>

#include <cassert>
#include <cmath>
#include <cstdio>

static constexpr float kEps = 1e-4f;

static bool approx_eq(float a, float b, float eps = kEps) {
    return std::fabs(a - b) < eps;
}

void test_identity() {
    fc::Quaternion q;
    assert(q.x() == 0.0f);
    assert(q.y() == 0.0f);
    assert(q.z() == 0.0f);
    assert(q.w() == 1.0f);

    fc::Quaternion q2 = fc::quaternion_identity();
    assert(q == q2);
    std::printf("PASS: Quaternion identity\n");
}

void test_component_constructor() {
    fc::Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    assert(q.x() == 1.0f);
    assert(q.y() == 2.0f);
    assert(q.z() == 3.0f);
    assert(q.w() == 4.0f);
    std::printf("PASS: Quaternion component constructor\n");
}

void test_element_access() {
    fc::Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    assert(q[0] == 1.0f);
    assert(q[1] == 2.0f);
    assert(q[2] == 3.0f);
    assert(q[3] == 4.0f);
    q[0] = 10.0f;
    assert(q.x() == 10.0f);
    std::printf("PASS: Quaternion element access\n");
}

void test_from_axis_angle_zero() {
    fc::Quaternion q = fc::Quaternion::from_axis_angle(
        fc::Vector3(0.0f, 0.0f, 1.0f), 0.0f
    );
    assert(approx_eq(q.x(), 0.0f));
    assert(approx_eq(q.y(), 0.0f));
    assert(approx_eq(q.z(), 0.0f));
    assert(approx_eq(q.w(), 1.0f));
    std::printf("PASS: Quaternion from_axis_angle zero\n");
}

void test_from_axis_angle_90z() {
    fc::Quaternion q = fc::Quaternion::from_axis_angle(
        fc::Vector3(0.0f, 0.0f, 1.0f),
        static_cast<float>(M_PI) / 2.0f
    );
    float s2 = static_cast<float>(M_SQRT2) / 2.0f;
    assert(approx_eq(q.x(), 0.0f));
    assert(approx_eq(q.y(), 0.0f));
    assert(approx_eq(q.z(), s2));
    assert(approx_eq(q.w(), s2));
    std::printf("PASS: Quaternion from_axis_angle 90z\n");
}

void test_from_euler_zero() {
    fc::Quaternion q = fc::Quaternion::from_euler(0.0f, 0.0f, 0.0f);
    assert(approx_eq(q.x(), 0.0f));
    assert(approx_eq(q.y(), 0.0f));
    assert(approx_eq(q.z(), 0.0f));
    assert(approx_eq(q.w(), 1.0f));
    std::printf("PASS: Quaternion from_euler zero\n");
}

void test_from_to_matrix_identity() {
    fc::Matrix3 m = fc::Matrix3::identity();
    fc::Quaternion q = fc::Quaternion::from_matrix(m);
    assert(approx_eq(q.x(), 0.0f));
    assert(approx_eq(q.y(), 0.0f));
    assert(approx_eq(q.z(), 0.0f));
    assert(approx_eq(q.w(), 1.0f));
    std::printf("PASS: Quaternion from/to matrix identity\n");
}

void test_to_matrix3_roundtrip() {
    fc::Quaternion orig = fc::Quaternion::from_axis_angle(
        fc::Vector3(0.0f, 1.0f, 0.0f), 1.2f
    );
    fc::Matrix3 m = orig.to_matrix3();
    fc::Quaternion back = fc::Quaternion::from_matrix(m);

    fc::Quaternion diff = orig * back.conjugate();
    assert(approx_eq(std::fabs(diff.w()), 1.0f, 1e-3f));
    std::printf("PASS: Quaternion to_matrix3 roundtrip\n");
}

void test_conjugate() {
    fc::Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    fc::Quaternion c = q.conjugate();
    assert(c.x() == -1.0f);
    assert(c.y() == -2.0f);
    assert(c.z() == -3.0f);
    assert(c.w() == 4.0f);
    std::printf("PASS: Quaternion conjugate\n");
}

void test_inverse() {
    fc::Quaternion q = fc::Quaternion::from_axis_angle(
        fc::Vector3(0.0f, 0.0f, 1.0f), 0.7f
    );
    fc::Quaternion inv = q.inverse();
    fc::Quaternion product = q * inv;
    assert(approx_eq(product.x(), 0.0f, 1e-3f));
    assert(approx_eq(product.y(), 0.0f, 1e-3f));
    assert(approx_eq(product.z(), 0.0f, 1e-3f));
    assert(approx_eq(product.w(), 1.0f, 1e-3f));
    std::printf("PASS: Quaternion inverse\n");
}

void test_length() {
    fc::Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    assert(approx_eq(q.length_squared(), 30.0f));
    assert(approx_eq(q.length(), std::sqrt(30.0f)));
    std::printf("PASS: Quaternion length\n");
}

void test_normalize() {
    fc::Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    fc::Quaternion n = q.normalized();
    assert(approx_eq(n.length(), 1.0f));
    std::printf("PASS: Quaternion normalize\n");
}

void test_multiply_identity() {
    fc::Quaternion id = fc::quaternion_identity();
    fc::Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    fc::Quaternion r = q * id;
    assert(r == q);

    fc::Quaternion r2 = id * q;
    assert(r2 == q);
    std::printf("PASS: Quaternion multiply identity\n");
}

void test_rotate_zero_angle() {
    fc::Quaternion id = fc::quaternion_identity();
    fc::Vector3 v(1.0f, 2.0f, 3.0f);
    fc::Vector3 r = id.rotate(v);
    assert(approx_eq(r.x(), 1.0f));
    assert(approx_eq(r.y(), 2.0f));
    assert(approx_eq(r.z(), 3.0f));
    std::printf("PASS: Quaternion rotate zero angle\n");
}

void test_rotate_90z() {
    fc::Quaternion q = fc::Quaternion::from_axis_angle(
        fc::Vector3(0.0f, 0.0f, 1.0f),
        static_cast<float>(M_PI) / 2.0f
    );
    fc::Vector3 v(1.0f, 0.0f, 0.0f);
    fc::Vector3 r = q.rotate(v);
    assert(approx_eq(r.x(), 0.0f));
    assert(approx_eq(r.y(), 1.0f, 1e-3f));
    assert(approx_eq(r.z(), 0.0f, 1e-3f));
    std::printf("PASS: Quaternion rotate 90z\n");
}

void test_to_axis_angle_roundtrip() {
    fc::Vector3 axis(0.0f, 1.0f, 0.0f);
    float angle = 1.5f;
    fc::Quaternion q = fc::Quaternion::from_axis_angle(axis, angle);

    fc::Vector3 out_axis;
    float out_angle;
    q.to_axis_angle(out_axis, out_angle);

    assert(approx_eq(out_axis.x(), 0.0f));
    assert(approx_eq(out_axis.y(), 1.0f));
    assert(approx_eq(out_angle, angle, 1e-3f));
    std::printf("PASS: Quaternion to_axis_angle roundtrip\n");
}

void test_to_euler_zero() {
    fc::Quaternion q = fc::quaternion_identity();
    float roll, pitch, yaw;
    q.to_euler(roll, pitch, yaw);
    assert(approx_eq(roll, 0.0f, 1e-3f));
    assert(approx_eq(pitch, 0.0f, 1e-3f));
    assert(approx_eq(yaw, 0.0f, 1e-3f));
    std::printf("PASS: Quaternion to_euler zero\n");
}

void test_to_matrix4() {
    fc::Quaternion q = fc::quaternion_identity();
    fc::Matrix4 m = q.to_matrix4();
    assert(m == fc::Matrix4::identity());
    std::printf("PASS: Quaternion to_matrix4\n");
}

void test_comparison() {
    fc::Quaternion a(1.0f, 2.0f, 3.0f, 4.0f);
    fc::Quaternion b(1.0f, 2.0f, 3.0f, 4.0f);
    fc::Quaternion c(5.0f, 6.0f, 7.0f, 8.0f);
    assert(a == b);
    assert(a != c);
    std::printf("PASS: Quaternion comparison\n");
}

void test_free_functions() {
    fc::Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);

    fc::Quaternion c1 = fc::conjugate(q);
    fc::Quaternion c2 = q.conjugate();
    assert(c1 == c2);

    fc::Quaternion i1 = fc::inverse(q);
    fc::Quaternion i2 = q.inverse();
    assert(approx_eq(i1.x(), i2.x()));
    assert(approx_eq(i1.y(), i2.y()));
    assert(approx_eq(i1.z(), i2.z()));
    assert(approx_eq(i1.w(), i2.w()));

    float d = fc::dot(q, q);
    assert(approx_eq(d, q.length_squared()));

    std::printf("PASS: Quaternion free functions\n");
}

void test_data_access() {
    fc::Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    const float* d = q.data();
    assert(d[0] == 1.0f);
    assert(d[1] == 2.0f);
    assert(d[2] == 3.0f);
    assert(d[3] == 4.0f);
    std::printf("PASS: Quaternion data access\n");
}

int main() {
    std::printf("Running Quaternion tests...\n");

    test_identity();
    test_component_constructor();
    test_element_access();
    test_from_axis_angle_zero();
    test_from_axis_angle_90z();
    test_from_euler_zero();
    test_from_to_matrix_identity();
    test_to_matrix3_roundtrip();
    test_conjugate();
    test_inverse();
    test_length();
    test_normalize();
    test_multiply_identity();
    test_rotate_zero_angle();
    test_rotate_90z();
    test_to_axis_angle_roundtrip();
    test_to_euler_zero();
    test_to_matrix4();
    test_comparison();
    test_free_functions();
    test_data_access();

    std::printf("All Quaternion tests passed.\n");
    return 0;
}
