/// @file test_header_includes.cpp
/// @brief Verifies all core headers compile independently.

#include <fc/core/compiler.hpp>
#include <fc/core/types/types.hpp>
#include <fc/core/status/error_code.hpp>
#include <fc/core/status/status.hpp>
#include <fc/core/status/result.hpp>
#include <fc/core/utilities/constants.hpp>
#include <fc/core/utilities/assert.hpp>
#include <fc/core/core.hpp>

#include <cstdio>

int main() {
    std::printf("All core headers compile successfully.\n");
    return 0;
}
