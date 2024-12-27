/**
 * Phosphor src/phosphor.cpp
 * This file is the main source file for the Phosphor library.
 */

#include "phosphor/phosphor.hpp"
#include "phosphor/window.hpp"
#include "phosphor/backends/renderer.hpp"
#include "phosphor/backends/opengl_renderer.hpp"
#include "phosphor/backends/vulkan_renderer.hpp"

#include <spdlog/spdlog.h>
#include <SDL2/SDL.h>

std::unique_ptr<Renderer> createRenderer(int type) {
    switch (type) {
        case 0:
            return std::make_unique<OpenGLRenderer>();
#ifdef PHOSPHOR_VULKAN
        case 1:
            return std::make_unique<VulkanRenderer>();
#endif
        default:
            return nullptr;
    }
}

void phosphor_init() {
    spdlog::info("Phosphor Engine v{}", get_phosphor_version());

#ifdef PHOSPHOR_VULKAN
    spdlog::info("Checking Vulkan support...");
#endif

    // Initialization
    Window* window;
    std::unique_ptr<Renderer> renderer;

    window = new Window();

#ifdef PHOSPHOR_VULKAN
    window->init(SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);
    renderer = createRenderer(1);

    if (renderer == nullptr) {
        spdlog::error("Failed to create renderer.");
        return;
    }

    renderer->set_window(window->getSDLWindow());
    renderer->init();
#else
    window->init(SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    renderer = createRenderer(0);

    if (renderer == nullptr) {
        spdlog::error("Failed to create renderer.");
        return;
    }

    renderer->set_window(window->getSDLWindow());
    renderer->init();
#endif

    // Main loop
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        renderer->render();
    }
    

    // Shutdown
    delete window;

    spdlog::info("Phosphor shut down");

}
