/// @file test_benchmarks.cpp
/// @brief Micro-benchmarks for math operations (manual timing, no external deps).

#include <fc/math/math.hpp>

#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdio>

static constexpr int kIterations = 100000;
static constexpr float kEps = 1e-4f;

static bool approx_eq(float a, float b, float eps = kEps) {
    return std::fabs(a - b) < eps;
}

using Clock = std::chrono::high_resolution_clock;

template <typename Func>
double measure_us(Func&& func, int iters = kIterations) {
    auto start = Clock::now();
    for (int i = 0; i < iters; ++i) {
        func();
    }
    auto end = Clock::now();
    double total_us = std::chrono::duration<double, std::micro>(end - start).count();
    return total_us / static_cast<double>(iters);
}

void bench_vector3_add() {
    fc::Vector3 a(1.0f, 2.0f, 3.0f);
    fc::Vector3 b(4.0f, 5.0f, 6.0f);
    fc::Vector3 result;
    double us = measure_us([&]() {
        result = a + b;
        asm volatile("" ::: "memory");
    });
    assert(result.x() == 5.0f);
    std::printf("  Vector3 add:          %8.3f us/op\n", us);
}

void bench_vector3_dot() {
    fc::Vector3 a(1.0f, 2.0f, 3.0f);
    fc::Vector3 b(4.0f, 5.0f, 6.0f);
    float result = 0.0f;
    double us = measure_us([&]() {
        result = a.dot(b);
        asm volatile("" ::: "memory");
    });
    assert(result == 32.0f);
    std::printf("  Vector3 dot:          %8.3f us/op\n", us);
}

void bench_vector3_cross() {
    fc::Vector3 a(1.0f, 2.0f, 3.0f);
    fc::Vector3 b(4.0f, 5.0f, 6.0f);
    fc::Vector3 result;
    double us = measure_us([&]() {
        result = a.cross(b);
        asm volatile("" ::: "memory");
    });
    assert(result.x() == -3.0f);
    std::printf("  Vector3 cross:        %8.3f us/op\n", us);
}

void bench_vector3_normalize() {
    fc::Vector3 a(1.0f, 2.0f, 3.0f);
    fc::Vector3 result;
    double us = measure_us([&]() {
        result = a.normalized();
        asm volatile("" ::: "memory");
    });
    assert(approx_eq(result.length(), 1.0f));
    std::printf("  Vector3 normalize:    %8.3f us/op\n", us);
}

void bench_matrix3_multiply() {
    fc::Matrix3 a = fc::Matrix3::rotation_x(0.5f);
    fc::Matrix3 b = fc::Matrix3::rotation_y(0.5f);
    fc::Matrix3 result;
    double us = measure_us([&]() {
        result = a * b;
        asm volatile("" ::: "memory");
    });
    assert(result(0, 0) != 0.0f);
    std::printf("  Matrix3 multiply:     %8.3f us/op\n", us);
}

void bench_matrix3_inverse() {
    fc::Matrix3 m = fc::Matrix3::rotation_x(0.5f);
    fc::Matrix3 result;
    double us = measure_us([&]() {
        result = m.inverse();
        asm volatile("" ::: "memory");
    });
    assert(result(0, 0) != 0.0f);
    std::printf("  Matrix3 inverse:      %8.3f us/op\n", us);
}

void bench_matrix4_multiply() {
    fc::Matrix4 a = fc::Matrix4::rotation_x(0.5f);
    fc::Matrix4 b = fc::Matrix4::rotation_y(0.5f);
    fc::Matrix4 result;
    double us = measure_us([&]() {
        result = a * b;
        asm volatile("" ::: "memory");
    });
    assert(result(0, 0) != 0.0f);
    std::printf("  Matrix4 multiply:     %8.3f us/op\n", us);
}

void bench_quaternion_multiply() {
    fc::Quaternion a = fc::Quaternion::from_axis_angle(
        fc::Vector3(0.0f, 0.0f, 1.0f), 0.5f);
    fc::Quaternion b = fc::Quaternion::from_axis_angle(
        fc::Vector3(0.0f, 1.0f, 0.0f), 0.5f);
    fc::Quaternion result;
    double us = measure_us([&]() {
        result = a * b;
        asm volatile("" ::: "memory");
    });
    assert(result.w() != 0.0f);
    std::printf("  Quaternion multiply:  %8.3f us/op\n", us);
}

void bench_quaternion_rotate() {
    fc::Quaternion q = fc::Quaternion::from_axis_angle(
        fc::Vector3(0.0f, 0.0f, 1.0f), 1.0f);
    fc::Vector3 v(1.0f, 2.0f, 3.0f);
    fc::Vector3 result;
    double us = measure_us([&]() {
        result = q.rotate(v);
        asm volatile("" ::: "memory");
    });
    assert(result.x() != 0.0f);
    std::printf("  Quaternion rotate:    %8.3f us/op\n", us);
}

void bench_quaternion_slerp() {
    fc::Quaternion a = fc::Quaternion::from_axis_angle(
        fc::Vector3(0.0f, 0.0f, 1.0f), 0.0f);
    fc::Quaternion b = fc::Quaternion::from_axis_angle(
        fc::Vector3(0.0f, 0.0f, 1.0f), 1.5f);
    fc::Quaternion result;
    double us = measure_us([&]() {
        result = a.slerp(b, 0.5f);
        asm volatile("" ::: "memory");
    });
    assert(result.w() != 0.0f);
    std::printf("  Quaternion slerp:     %8.3f us/op\n", us);
}

int main() {
    std::printf("Running math benchmarks (%d iterations each)...\n", kIterations);

    bench_vector3_add();
    bench_vector3_dot();
    bench_vector3_cross();
    bench_vector3_normalize();
    bench_matrix3_multiply();
    bench_matrix3_inverse();
    bench_matrix4_multiply();
    bench_quaternion_multiply();
    bench_quaternion_rotate();
    bench_quaternion_slerp();

    std::printf("All benchmarks completed.\n");
    return 0;
}
