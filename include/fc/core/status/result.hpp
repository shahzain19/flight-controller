/// @file result.hpp
/// @brief Result<T> class for returning values or errors without exceptions.
///
/// Result<T> holds either a value of type T or an error Status. It replaces
/// the use of exceptions for error handling in the flight computer.
///
/// Design:
///   - No heap allocation
///   - No exceptions
///   - T must be destructible
///   - sizeof(Result<T>) == sizeof(Storage) + sizeof(bool) + padding
///
/// Usage:
///   Result<f64> divide(f64 a, f64 b) {
///       if (b == 0.0) return Result<f64>::error(ErrorCode::kDivisionByZero);
///       return Result<f64>::ok(a / b);
///   }
///
///   auto result = divide(10.0, 2.0);
///   if (result) {
///       f64 value = result.value();
///   } else {
///       Status error = result.status();
///   }
///
/// @defgroup core_result Result
/// @{

#pragma once

#include "status.hpp"
#include "../types/types.hpp"
#include "../compiler.hpp"

#include <new>
#include <type_traits>
#include <utility>

namespace fc {

/// @brief Result type that holds either a value or an error.
///
/// This class is the primary mechanism for returning values from functions
/// that may fail. It eliminates the need for exceptions while providing
/// type-safe error propagation.
///
/// @tparam T The value type. Must be destructible.
template <typename T>
class Result {
    static_assert(!std::is_reference_v<T>, "Result<T> cannot hold a reference type");
    static_assert(!std::is_array_v<T>, "Result<T> cannot hold an array type");
    static_assert(std::is_destructible_v<T>, "T must be destructible");

public:
    using value_type = T;

    // ========================================================================
    // Factory Methods (must be defined after Storage)
    // ========================================================================

    /// @brief Constructs a Result holding a value (copy).
    ///
    /// @param value The value to store. Copied into the Result.
    FC_NODISCARD static constexpr Result ok(const T& value) noexcept(
        std::is_nothrow_copy_constructible_v<T>) {
        Result result;
        ::new (&result.storage_.value) T(value);
        result.has_value_ = true;
        return result;
    }

    /// @brief Constructs a Result holding a value (move).
    ///
    /// @param value The value to store. Moved into the Result.
    FC_NODISCARD static constexpr Result ok(T&& value) noexcept(
        std::is_nothrow_move_constructible_v<T>) {
        Result result;
        ::new (&result.storage_.value) T(std::move(value));
        result.has_value_ = true;
        return result;
    }

    /// @brief Constructs a Result holding an error.
    ///
    /// @param status The error status.
    FC_NODISCARD static constexpr Result error(Status status) noexcept {
        Result result;
        ::new (&result.storage_.error) Status(status);
        result.has_value_ = false;
        return result;
    }

    /// @brief Constructs a Result holding an error from an ErrorCode.
    ///
    /// @param code The error code.
    FC_NODISCARD static constexpr Result error(ErrorCode code) noexcept {
        return error(Status(code));
    }

    // ========================================================================
    // Copy and Move
    // ========================================================================

    /// @brief Copy constructor.
    Result(const Result& other) noexcept(std::is_nothrow_copy_constructible_v<T>) {
        if (other.has_value_) {
            ::new (&storage_.value) T(other.storage_.value);
            has_value_ = true;
        } else {
            ::new (&storage_.error) Status(other.storage_.error);
            has_value_ = false;
        }
    }

    /// @brief Move constructor.
    Result(Result&& other) noexcept(std::is_nothrow_move_constructible_v<T>) {
        if (other.has_value_) {
            ::new (&storage_.value) T(std::move(other.storage_.value));
            has_value_ = true;
        } else {
            ::new (&storage_.error) Status(std::move(other.storage_.error));
            has_value_ = false;
        }
    }

    /// @brief Copy assignment operator.
    Result& operator=(const Result& other) noexcept(
        std::is_nothrow_copy_constructible_v<T> &&
        std::is_nothrow_copy_assignable_v<T>) {
        if (this != &other) {
            destroy();
            if (other.has_value_) {
                ::new (&storage_.value) T(other.storage_.value);
                has_value_ = true;
            } else {
                ::new (&storage_.error) Status(other.storage_.error);
                has_value_ = false;
            }
        }
        return *this;
    }

    /// @brief Move assignment operator.
    Result& operator=(Result&& other) noexcept(
        std::is_nothrow_move_constructible_v<T> &&
        std::is_nothrow_move_assignable_v<T>) {
        if (this != &other) {
            destroy();
            if (other.has_value_) {
                ::new (&storage_.value) T(std::move(other.storage_.value));
                has_value_ = true;
            } else {
                ::new (&storage_.error) Status(std::move(other.storage_.error));
                has_value_ = false;
            }
        }
        return *this;
    }

    // ========================================================================
    // Destructor
    // ========================================================================

    /// @brief Destructor. Destroys the contained value or error.
    ~Result() {
        destroy();
    }

    // ========================================================================
    // Observers
    // ========================================================================

    /// @brief Returns true if the Result holds a value.
    [[nodiscard]] constexpr bool has_value() const noexcept {
        return has_value_;
    }

    /// @brief Returns true if the Result holds a value.
    [[nodiscard]] constexpr explicit operator bool() const noexcept {
        return has_value_;
    }

    /// @brief Returns a reference to the contained value.
    ///
    /// Behavior is undefined if the Result holds an error.
    /// Use has_value() or operator bool() to check before calling.
    [[nodiscard]] constexpr T& value() & noexcept {
        return storage_.value;
    }

    /// @brief Returns a const reference to the contained value.
    [[nodiscard]] constexpr const T& value() const& noexcept {
        return storage_.value;
    }

    /// @brief Returns the contained value by move.
    ///
    /// After this call, the Result is in a moved-from state.
    [[nodiscard]] constexpr T value() && noexcept(
        std::is_nothrow_move_constructible_v<T>) {
        return std::move(storage_.value);
    }

    /// @brief Returns the error status.
    ///
    /// Returns a default OK status if the Result holds a value.
    [[nodiscard]] constexpr Status status() const noexcept {
        if (has_value_) {
            return Status::ok();
        }
        return storage_.error;
    }

    /// @brief Returns the error code.
    ///
    /// Returns ErrorCode::kOk if the Result holds a value.
    [[nodiscard]] constexpr ErrorCode error_code() const noexcept {
        return status().code();
    }

    // ========================================================================
    // Value Access Operators
    // ========================================================================

    /// @brief Returns a pointer to the contained value.
    ///
    /// Returns nullptr if the Result holds an error.
    [[nodiscard]] constexpr T* operator->() noexcept {
        return has_value_ ? &storage_.value : nullptr;
    }

    /// @brief Returns a const pointer to the contained value.
    [[nodiscard]] constexpr const T* operator->() const noexcept {
        return has_value_ ? &storage_.value : nullptr;
    }

    /// @brief Returns a reference to the contained value.
    ///
    /// Behavior is undefined if the Result holds an error.
    [[nodiscard]] constexpr T& operator*() & noexcept {
        return storage_.value;
    }

    /// @brief Returns a const reference to the contained value.
    [[nodiscard]] constexpr const T& operator*() const& noexcept {
        return storage_.value;
    }

    // ========================================================================
    // Equality Operators
    // ========================================================================

    /// @brief Two Results are equal if both hold values and the values are equal,
    ///        or both hold errors with the same error code.
    template <typename U>
    [[nodiscard]] constexpr bool operator==(const Result<U>& other) const noexcept {
        if (has_value_ != other.has_value_) return false;
        if (has_value_) return storage_.value == other.storage_.value;
        return storage_.error.code() == other.storage_.error.code();
    }

    /// @brief Two Results are unequal if they differ.
    template <typename U>
    [[nodiscard]] constexpr bool operator!=(const Result<U>& other) const noexcept {
        return !(*this == other);
    }

    /// @brief Compares a Result with a Status (for error checking).
    [[nodiscard]] constexpr bool operator==(Status status) const noexcept {
        if (has_value_) return status.is_ok();
        return storage_.error.code() == status.code();
    }

    /// @brief Compares a Result with an ErrorCode (for error checking).
    [[nodiscard]] constexpr bool operator==(ErrorCode code) const noexcept {
        if (has_value_) return code == ErrorCode::kOk;
        return storage_.error.code() == code;
    }

    /// @brief Compares a Result with an ErrorCode (for error checking).
    [[nodiscard]] constexpr bool operator!=(ErrorCode code) const noexcept {
        return !(*this == code);
    }

private:
    /// @brief Storage union for value or error.
    ///
    /// The union is never in an active state by default; placement new
    /// is used to construct the appropriate member.
    union Storage {
        /// @brief Default constructor does nothing (union is uninitialized).
        constexpr Storage() noexcept {}
        /// @brief Destructor does nothing (Result::destroy() handles cleanup).
        ~Storage() {}

        /// @brief Value member. Active when has_value_ is true.
        alignas(T) T value;
        /// @brief Error member. Active when has_value_ is false.
        Status error;
    };

    /// @brief Private default constructor for factory methods.
    /// The storage is left uninitialized; factory methods use placement new.
    constexpr Result() noexcept : storage_{}, has_value_(false) {}

    /// @brief Destroys the contained value or error.
    constexpr void destroy() noexcept {
        if (has_value_) {
            storage_.value.~T();
        } else {
            storage_.error.~Status();
        }
    }

    Storage storage_;
    bool has_value_;
};

// ============================================================================
// Factory Functions
// ============================================================================

/// @brief Creates a Result holding a value.
template <typename T>
[[nodiscard]] constexpr Result<std::decay_t<T>> make_ok(T&& value) noexcept(
    std::is_nothrow_constructible_v<std::decay_t<T>, T>) {
    return Result<std::decay_t<T>>::ok(std::forward<T>(value));
}

/// @brief Creates a Result holding an error.
[[nodiscard]] constexpr Result<int> make_error(ErrorCode code) noexcept {
    return Result<int>::error(code);
}

/// @brief Creates a Result holding an error with a message.
[[nodiscard]] constexpr Result<int> make_error(ErrorCode code, const char* message) noexcept {
    return Result<int>::error(Status(code, message));
}

} // namespace fc

/// @}
