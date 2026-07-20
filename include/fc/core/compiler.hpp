/// @file compiler.hpp
/// @brief Compiler detection, attribute definitions, and warning control.
///
/// This header provides portable macros for compiler-specific features,
/// attributes, and pragma control. It is included by all other core headers
/// to ensure consistent compiler behavior across the codebase.
///
/// Compliance:
///   - No undefined behavior
///   - No compiler-specific extensions without documentation
///   - All macros are constexpr-safe and header-only
///
/// @defgroup core_compiler Compiler Configuration
/// @{

#pragma once

// ============================================================================
// Compiler Detection
// ============================================================================

/// @def FC_COMPILER_GCC
/// Defined to the compiler version (encoded as integer) when using GCC.
#if defined(__GNUC__) && !defined(__clang__)
    #define FC_COMPILER_GCC (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

/// @def FC_COMPILER_CLANG
/// Defined to the compiler version (encoded as integer) when using Clang.
#if defined(__clang__)
    #define FC_COMPILER_CLANG (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#endif

/// @def FC_COMPILER_MSVC
/// Defined to the compiler version (encoded as integer) when using MSVC.
#if defined(_MSC_VER)
    #define FC_COMPILER_MSVC _MSC_VER
#endif

// ============================================================================
// C++ Standard Detection
// ============================================================================

/// @def FC_CXX_STANDARD
/// The detected C++ standard version as an integer (e.g., 202302 for C++23).
#if defined(__cplusplus)
    #define FC_CXX_STANDARD __cplusplus
#endif

// ============================================================================
// Warning Control Pragmas
// ============================================================================

/// @def FC_PUSH_WARNINGS
/// Pushes the current warning state onto the pragma stack.
#if defined(FC_COMPILER_GCC) || defined(FC_COMPILER_CLANG)
    #define FC_PUSH_WARNINGS _Pragma("GCC diagnostic push")
#elif defined(FC_COMPILER_MSVC)
    #define FC_PUSH_WARNINGS __pragma(warning(push))
#else
    #define FC_PUSH_WARNINGS /* unsupported compiler */
#endif

/// @def FC_POP_WARNINGS
/// Restores the warning state from the pragma stack.
#if defined(FC_COMPILER_GCC) || defined(FC_COMPILER_CLANG)
    #define FC_POP_WARNINGS _Pragma("GCC diagnostic pop")
#elif defined(FC_COMPILER_MSVC)
    #define FC_POP_WARNINGS __pragma(warning(pop))
#else
    #define FC_POP_WARNINGS /* unsupported compiler */
#endif

/// @def FC_DISABLE_WARNING(name)
/// Disables a specific compiler warning for the enclosing scope.
/// @param name The warning name (GCC/Clang) or number (MSVC).
#if defined(FC_COMPILER_GCC) || defined(FC_COMPILER_CLANG)
    #define FC_DISABLE_WARNING_IMPL(w) _Pragma(#w)
    #define FC_DISABLE_WARNING(name) FC_DISABLE_WARNING_IMPL(GCC diagnostic ignored name)
#elif defined(FC_COMPILER_MSVC)
    #define FC_DISABLE_WARNING_IMPL(num) __pragma(warning(disable : num))
    #define FC_DISABLE_WARNING(name) FC_DISABLE_WARNING_IMPL(name)
#else
    #define FC_DISABLE_WARNING(name) /* unsupported compiler */
#endif

// ============================================================================
// Compiler Attributes
// ============================================================================

/// @def FC_CONSTEXPR
/// Expands to constexpr in C++20 and later, empty otherwise.
#if defined(FC_CXX_STANDARD) && (FC_CXX_STANDARD >= 202002L)
    #define FC_CONSTEXPR constexpr
#else
    #define FC_CONSTEXPR
#endif

/// @def FC_CONSTEVAL
/// Expands to consteval in C++20 and later, constexpr otherwise.
#if defined(FC_CXX_STANDARD) && (FC_CXX_STANDARD >= 202002L)
    #define FC_CONSTEVAL consteval
#else
    #define FC_CONSTEVAL constexpr
#endif

/// @def FC_NODISCARD
/// Expands to [[nodiscard]] in C++17 and later.
#if defined(FC_CXX_STANDARD) && (FC_CXX_STANDARD >= 201703L)
    #define FC_NODISCARD [[nodiscard]]
#else
    #define FC_NODISCARD
#endif

/// @def FC_NORETURN
/// Expands to [[noreturn]] in C++11 and later.
#if defined(FC_CXX_STANDARD) && (FC_CXX_STANDARD >= 201103L)
    #define FC_NORETURN [[noreturn]]
#else
    #define FC_NORETURN
#endif

/// @def FC_DEPRECATED
/// Marks a function or variable as deprecated.
#if defined(FC_CXX_STANDARD) && (FC_CXX_STANDARD >= 201402L)
    #define FC_DEPRECATED [[deprecated]]
#else
    #define FC_DEPRECATED
#endif

/// @def FC_LIKELY / FC_UNLIKELY
/// Branch prediction hints.
#if defined(FC_COMPILER_GCC) || defined(FC_COMPILER_CLANG)
    #define FC_LIKELY(x) __builtin_expect(!!(x), 1)
    #define FC_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
    #define FC_LIKELY(x) (!!(x))
    #define FC_UNLIKELY(x) (!!(x))
#endif

/// @def FC_ASSUME(cond)
/// Tells the compiler that a condition is always true.
/// Undefined behavior if the condition is actually false.
#if defined(FC_COMPILER_GCC) || defined(FC_COMPILER_CLANG)
    #define FC_ASSUME(cond) __builtin_assume(cond)
#elif defined(FC_COMPILER_MSVC)
    #define FC_ASSUME(cond) __assume(cond)
#else
    #define FC_ASSUME(cond) ((void)(cond))
#endif

/// @def FC_UNREACHABLE
/// Marks a point in code that should never be reached.
#if defined(FC_COMPILER_GCC) || defined(FC_COMPILER_CLANG)
    #define FC_UNREACHABLE __builtin_unreachable()
#elif defined(FC_COMPILER_MSVC)
    #define FC_UNREACHABLE __assume(false)
#else
    #define FC_UNREACHABLE ((void)0)
#endif

/// @def FC_FUNCTION_NAME
/// Expands to the name of the enclosing function, if available.
#if defined(FC_COMPILER_GCC) || defined(FC_COMPILER_CLANG)
    #define FC_FUNCTION_NAME __PRETTY_FUNCTION__
#elif defined(FC_COMPILER_MSVC)
    #define FC_FUNCTION_NAME __FUNCSIG__
#else
    #define FC_FUNCTION_NAME __func__
#endif

// ============================================================================
// Platform Detection
// ============================================================================

/// @def FC_PLATFORM_LINUX
/// Defined when compiling for Linux.
#if defined(__linux__)
    #define FC_PLATFORM_LINUX 1
#endif

/// @def FC_PLATFORM_MACOS
/// Defined when compiling for macOS.
#if defined(__APPLE__)
    #define FC_PLATFORM_MACOS 1
#endif

/// @def FC_PLATFORM_WINDOWS
/// Defined when compiling for Windows.
#if defined(_WIN32)
    #define FC_PLATFORM_WINDOWS 1
#endif

/// @def FC_PLATFORM_EMBEDDED
/// Defined when compiling for a bare-metal or embedded target.
#if defined(__sparc__) || defined(__arm__) || defined(__aarch64__) || \
    defined(__riscv) || defined(__AVR__) || defined(__XTENSA__)
    #define FC_PLATFORM_EMBEDDED 1
#endif

// ============================================================================
// Architecture Detection
// ============================================================================

/// @def FC_ARCH_X86_64
/// Defined when targeting x86-64 architecture.
#if defined(__x86_64__) || defined(_M_X64)
    #define FC_ARCH_X86_64 1
#endif

/// @def FC_ARCH_AARCH64
/// Defined when targeting AArch64 (ARM 64-bit) architecture.
#if defined(__aarch64__) || defined(_M_ARM64)
    #define FC_ARCH_AARCH64 1
#endif

/// @def FC_ARCH_ARM32
/// Defined when targeting ARM 32-bit architecture.
#if defined(__arm__) && !defined(__aarch64__)
    #define FC_ARCH_ARM32 1
#endif

/// @def FC_ARCH_RISCV
/// Defined when targeting RISC-V architecture.
#if defined(__riscv)
    #define FC_ARCH_RISCV 1
#endif

/// @}
