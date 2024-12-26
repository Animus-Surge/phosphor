#pragma once

/**
 * Phosphor include/phosphor/phosphor.hpp
 * This file is the main include file for the Phosphor library.
 */

#include <string>

#define VULKAN_HPP_NO_CONSTRUCTORS // Use c++20 constructors

#define PHOSPHOR_VERSION_MAJOR 0
#define PHOSPHOR_VERSION_MINOR 1
#define PHOSPHOR_VERSION_PATCH 0

/**
 * Get the version of the Phosphor library.
 * @return The version of the Phosphor library.
 */
std::string get_phosphor_version() {
    return std::to_string(PHOSPHOR_VERSION_MAJOR) + "." + std::to_string(PHOSPHOR_VERSION_MINOR) + "." + std::to_string(PHOSPHOR_VERSION_PATCH);
}


/**
 * Initialize the Phosphor library.
 */
void phosphor_init();
