/**
 * Phosphor phosphor.cpp
 *
 * Entry point and initialization for the Phosphor engine
 */

#include <cstdio>

#include "phosphor/phosphor.h"

void phosphor_init() {
    printf("Phosphor %s\n", PHOSPHOR_VERSION_STRING);

    // Initialize the renderer
    std::unique_ptr<Renderer> renderer = create_renderer(0);
    renderer->init();
    renderer->run();
    renderer->shutdown();

    printf("Phosphor shutting down\n");
}
