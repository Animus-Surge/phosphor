/**
 * Phosphor src/phosphor.cpp
 * This file is the main source file for the Phosphor library.
 */

#include "phosphor/phosphor.hpp"

#include <spdlog/spdlog.h>

namespace Phosphor {

    void init() {
        spdlog::info("Hello, World!");
    }
};
