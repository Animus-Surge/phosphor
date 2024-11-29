#pragma once

/*
 * Phosphor phosphor/phosphor.h
 * Main header file
 *
 * Contains declarations for all engine systems
 */

#define PHOSPHOR_VERSION_MAJOR 0
#define PHOSPHOR_VERSION_MINOR 1
#define PHOSPHOR_VERSION_PATCH 1

#define PHOSPHOR_DEBUG 1
#define PHOSPHOR_ALPHA 0
#define PHOSPHOR_BETA 0
#define PHOSPHOR_RELEASE 0

#define PHOSPHOR_VERSION_STRING "0.1.1 (dev)"

#include <string>

#include "phosphor/renderer.h"

// Entry point

void phosphor_init();

static inline std::string phosphor_version() {
    return "Phosphor " + std::to_string(PHOSPHOR_VERSION_MAJOR) + "." + std::to_string(PHOSPHOR_VERSION_MINOR) + "." + std::to_string(PHOSPHOR_VERSION_PATCH)
        + (PHOSPHOR_ALPHA ? " alpha" : "") + (PHOSPHOR_BETA ? " beta" : "") + (PHOSPHOR_RELEASE ? " release" : "") + (PHOSPHOR_DEBUG ? " (dev)" : "");
}

