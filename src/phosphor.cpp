/**
 * Phosphor src/phosphor.cpp
 * This file is the main source file for the Phosphor library.
 */

#include "phosphor/phosphor.hpp"
#include "phosphor/window.hpp"
#include "phosphor/backends/vulkan_renderer.hpp"

#include <spdlog/spdlog.h>
#include <SDL2/SDL.h>


void phosphor_init() {
    spdlog::info("Phosphor Engine v{}", get_phosphor_version());

    // Initialization
    Window* window;

    window = new Window();
    window->init(SDL_WINDOW_SHOWN);

    VulkanRenderer* renderer = new VulkanRenderer();
    renderer->set_window(window->getSDLWindow());
    renderer->init();

    // Main loop

    // Shutdown
    delete window;

    spdlog::info("Phosphor shut down");

}
