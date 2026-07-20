/// @file status.hpp
/// @brief Status class for error handling without exceptions.
///
/// Every function that can fail returns a Status object. The Status holds
/// an ErrorCode and an optional message. Functions that succeed return
/// Status::ok().
///
/// Design:
///   - No heap allocation (message is a pointer to a string literal)
///   - No exceptions
///   - constexpr where possible
///   - sizeof(Status) == 16 bytes (on 64-bit platforms)
///
/// Usage:
///   Status do_work() {
///       if (invalid_input) return Status(ErrorCode::kInvalidArgument);
///       // ... work ...
///       return Status::ok();
///   }
///
///   Status result = do_work();
///   if (!result.is_ok()) { handle_error(result); }
///
/// @defgroup core_status Status
/// @{

#pragma once

#include "error_code.hpp"
#include "../compiler.hpp"

namespace fc {

/// @brief Status class for error handling.
///
/// This class represents the outcome of an operation that may fail.
/// It contains an error code and an optional message string.
///
/// Memory layout: 16 bytes on 64-bit, 8 bytes on 32-bit.
/// No dynamic allocation. No exceptions.
class Status {
public:
    // ========================================================================
    // Constructors
    // ========================================================================

    /// @brief Default constructor. Creates an OK status.
    constexpr Status() noexcept = default;

    /// @brief Constructs a Status with an error code.
    ///
    /// @param code The error code. ErrorCode::kOk represents success.
    constexpr explicit Status(ErrorCode code) noexcept
        : code_(code), message_(nullptr) {}

    /// @brief Constructs a Status with an error code and message.
    ///
    /// @param code The error code.
    /// @param message A string literal. Must outlive the Status object.
    ///                Caller ensures the string is valid for the program lifetime.
    constexpr Status(ErrorCode code, const char* message) noexcept
        : code_(code), message_(message) {}

    // ========================================================================
    // Factory Methods
    // ========================================================================

    /// @brief Creates an OK status.
    [[nodiscard]] static constexpr Status ok() noexcept {
        return Status(ErrorCode::kOk);
    }

    /// @brief Creates an error status with a message.
    ///
    /// @param code The error code. Must not be ErrorCode::kOk.
    /// @param message A string literal.
    [[nodiscard]] static constexpr Status error(ErrorCode code, const char* message) noexcept {
        return Status(code, message);
    }

    // ========================================================================
    // Observers
    // ========================================================================

    /// @brief Returns true if the status represents success.
    [[nodiscard]] constexpr bool is_ok() const noexcept {
        return code_ == ErrorCode::kOk;
    }

    /// @brief Returns the error code.
    [[nodiscard]] constexpr ErrorCode code() const noexcept {
        return code_;
    }

    /// @brief Returns the error message, or nullptr if no message was set.
    [[nodiscard]] constexpr const char* message() const noexcept {
        return message_;
    }

    /// @brief Returns the error code as a human-readable string.
    [[nodiscard]] constexpr const char* code_string() const noexcept {
        return error_code_to_string(code_);
    }

    /// @brief Returns true if the status represents an error.
    [[nodiscard]] constexpr bool is_error() const noexcept {
        return fc::is_error(code_);
    }

    // ========================================================================
    // Comparison Operators
    // ========================================================================

    /// @brief Two Status objects are equal if they have the same error code.
    [[nodiscard]] constexpr bool operator==(const Status& other) const noexcept {
        return code_ == other.code_;
    }

    /// @brief Two Status objects are unequal if they have different error codes.
    [[nodiscard]] constexpr bool operator!=(const Status& other) const noexcept {
        return code_ != other.code_;
    }

    /// @brief Compares a Status with an ErrorCode.
    [[nodiscard]] constexpr bool operator==(ErrorCode code) const noexcept {
        return code_ == code;
    }

    /// @brief Compares a Status with an ErrorCode.
    [[nodiscard]] constexpr bool operator!=(ErrorCode code) const noexcept {
        return code_ != code;
    }

    // ========================================================================
    // Conversion Operators
    // ========================================================================

    /// @brief Explicit conversion to bool. Returns true if the status is OK.
    [[nodiscard]] constexpr explicit operator bool() const noexcept {
        return is_ok();
    }

private:
    ErrorCode code_ = ErrorCode::kOk;
    const char* message_ = nullptr;
};

// ============================================================================
// Free Functions
// ============================================================================

/// @brief Returns true if the status represents success.
[[nodiscard]] constexpr bool is_ok(const Status& status) noexcept {
    return status.is_ok();
}

/// @brief Returns true if the status represents an error.
[[nodiscard]] constexpr bool is_error(const Status& status) noexcept {
    return status.is_error();
}

} // namespace fc

/// @}
