/// @file core.hpp
/// @brief Umbrella header for the Flight Computer core module.
///
/// This header includes all public core headers. Include this file
/// when you need access to the full core module API.
///
/// Usage:
///   #include <fc/core/core.hpp>
///
/// @defgroup core Core Module
/// @{

#pragma once

// Compiler configuration (must be first)
#include "compiler.hpp"

// Types
#include "types/types.hpp"

// Status and error handling
#include "status/error_code.hpp"
#include "status/status.hpp"
#include "status/result.hpp"

// Utilities
#include "utilities/constants.hpp"
#include "utilities/assert.hpp"

/// @}
