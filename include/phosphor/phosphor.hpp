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

#ifdef _WIN32
#define PHOSPHOR_EXPORT __declspec(dllexport)
#define PHOSPHOR_IMPORT __declspec(dllimport)
#elif defined(__GNUC__) || defined(__clang__)
#define PHOSPHOR_EXPORT __attribute__((visibility("default")))
#define PHOSPHOR_IMPORT
#else
#define PHOSPHOR_EXPORT
#define PHOSPHOR_IMPORT
#endif

/**
 * Get the version of the Phosphor library.
 * @return The version of the Phosphor library.
 */
inline PHOSPHOR_EXPORT std::string get_phosphor_version() {
    return std::to_string(PHOSPHOR_VERSION_MAJOR) + "." + std::to_string(PHOSPHOR_VERSION_MINOR) + "." + std::to_string(PHOSPHOR_VERSION_PATCH);
}

class Engine {
protected:
    std::string name;

public:
    explicit Engine(const std::string &name) : name(name) {}
    ~Engine() {
        //TODO: cleanup
    }

    void init();
    void run();
};

/**
 * Initialize the Phosphor library.
 */
void PHOSPHOR_EXPORT phosphor_init();
