cmake_minimum_required(VERSION 3.21)

# ============================================================================
# Project Configuration
# ============================================================================

project(FlightComputer
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION "Flight Computer Software Stack"
)

# ============================================================================
# C++ Standard
# ============================================================================

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================================
# Build Profiles
# ============================================================================

set(FC_PROFILE "simulation" CACHE STRING "Build profile: simulation, development, production")
set_property(CACHE FC_PROFILE PROPERTY STRINGS simulation development production)

if(NOT FC_PROFILE MATCHES "^(simulation|development|production)$")
    message(FATAL_ERROR "Invalid FC_PROFILE: ${FC_PROFILE}. Must be simulation, development, or production.")
endif()

message(STATUS "Build profile: ${FC_PROFILE}")

# ============================================================================
# Compiler Warnings
# ============================================================================

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -Wshadow
    -Wconversion
    -Wsign-conversion
    -Wdouble-promotion
    -Wformat=2
    -Wnon-virtual-dtor
    -Woverloaded-virtual
    -Wcast-align
    -Wcast-qual
    -Wold-style-cast
    -Wuseless-cast
    -Wnull-dereference
    -Wimplicit-fallthrough
    -Wswitch-enum
    -Wswitch-default
)

# ============================================================================
# Debug / Release Settings
# ============================================================================

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(FC_DEBUG=1)
    add_compile_options(-g -O0)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(FC_NDEBUG=1)
    add_compile_options(-O2 -DNDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_options(-O2 -g)
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    add_compile_options(-Os)
endif()

# ============================================================================
# Profile Definitions
# ============================================================================

if(FC_PROFILE STREQUAL "simulation")
    add_compile_definitions(FC_PROFILE_SIMULATION=1)
    add_compile_definitions(FC_ENABLE_SIMULATION=1)
elseif(FC_PROFILE STREQUAL "development")
    add_compile_definitions(FC_PROFILE_DEVELOPMENT=1)
    add_compile_definitions(FC_ENABLE_DEBUG_LOGGING=1)
elseif(FC_PROFILE STREQUAL "production")
    add_compile_definitions(FC_PROFILE_PRODUCTION=1)
    add_compile_definitions(FC_NDEBUG=1)
endif()

# ============================================================================
# Coverage
# ============================================================================

option(FC_ENABLE_COVERAGE "Enable code coverage" OFF)

if(FC_ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
        add_link_options(--coverage)
    else()
        message(WARNING "Coverage not supported for this compiler")
    endif()
endif()

# ============================================================================
# Include Directories
# ============================================================================

include_directories(${CMAKE_SOURCE_DIR}/include)

# ============================================================================
# Core Module (Header-Only)
# ============================================================================

add_library(fc_core INTERFACE)
target_include_directories(fc_core INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(fc_core INTERFACE cxx_std_23)

# ============================================================================
# Tests
# ============================================================================

option(FC_BUILD_TESTS "Build test suite" ON)

if(FC_BUILD_TESTS)
    enable_testing()

    # Test executable template
    function(fc_add_test test_name test_source)
        add_executable(${test_name} ${test_source})
        target_link_libraries(${test_name} PRIVATE fc_core)
        target_compile_definitions(${test_name} PRIVATE
            $<$<CONFIG:Debug>:FC_DEBUG=1>
            $<$<CONFIG:Release>:FC_NDEBUG=1>
        )
        add_test(NAME ${test_name} COMMAND ${test_name})
    endfunction()

    # Unit tests
    fc_add_test(test_types tests/unit/core/test_types.cpp)
    fc_add_test(test_error_code tests/unit/core/test_error_code.cpp)
    fc_add_test(test_status tests/unit/core/test_status.cpp)
    fc_add_test(test_result tests/unit/core/test_result.cpp)
    fc_add_test(test_constants tests/unit/core/test_constants.cpp)
    fc_add_test(test_assert tests/unit/core/test_assert.cpp)

    # Ensure all headers compile independently
    fc_add_test(test_header_includes tests/unit/core/test_header_includes.cpp)

    # Math tests
    fc_add_test(test_vector2 tests/unit/math/test_vector2.cpp)
    fc_add_test(test_vector3 tests/unit/math/test_vector3.cpp)
    fc_add_test(test_matrix3 tests/unit/math/test_matrix3.cpp)
    fc_add_test(test_matrix4 tests/unit/math/test_matrix4.cpp)
    fc_add_test(test_quaternion tests/unit/math/test_quaternion.cpp)

    # Benchmarks
    fc_add_test(test_benchmarks tests/unit/math/test_benchmarks.cpp)
endif()
