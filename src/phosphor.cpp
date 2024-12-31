/**
 * Phosphor src/phosphor.cpp
 * This file is the main source file for the Phosphor library.
 */

#include "phosphor/phosphor.hpp"

#include <phosphor/camera.hpp>

#include "phosphor/imgui.hpp"
#include "phosphor/mesh/mesh.hpp"
#include "phosphor/window.hpp"
#include "phosphor/backends/renderer.hpp"
#include "phosphor/backends/opengl_renderer.hpp"
#include "phosphor/backends/vulkan_renderer.hpp"
#include "phosphor/event.hpp"

#include <spdlog/spdlog.h>
#include <SDL2/SDL.h>

bool running = true;

std::unique_ptr<Renderer> createRenderer(const int type) {
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

void callback_quit() {
    running = false;
}

void phosphor_init() {
    spdlog::info("Phosphor Engine v{}", get_phosphor_version());

#ifdef PHOSPHOR_VULKAN
    spdlog::info("Checking Vulkan support...");
#endif

    // Initialization
    std::unique_ptr<Renderer> renderer;

    auto *window = new Window();

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


    auto* scene = new Scene();
    auto* object = new Object();
    auto* mesh = create_primitive_box(1.0f, 1.0f, 1.0f);

    auto* camera = new Camera(window->getWidth(), window->getHeight());

    scene->addObject(object);
    object->addComponent(mesh);

    renderer->set_scene(scene);

    add_key_callback(SDL_SCANCODE_ESCAPE, SDL_PRESSED, callback_quit);
    add_system_callback(SDL_QUIT, callback_quit);

    float deltaT = 0.0f;

    bool mouse_captured = false;

    // Main loop
    while (running) {
        const float start = SDL_GetTicks();

        if (mouse_captured) {
            SDL_CaptureMouse(SDL_TRUE);
            SDL_SetRelativeMouseMode(SDL_TRUE);
        } else {
            SDL_CaptureMouse(SDL_FALSE);
            SDL_SetRelativeMouseMode(SDL_FALSE);
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            imgui_processEvent(&event);
            handle_event(&event);

            if (event.type == SDL_MOUSEMOTION) {
                if (mouse_captured) {
                    int dx = event.motion.xrel;
                    int dy = event.motion.yrel;

                    camera->rotate_x(-dy * 0.01f);
                    camera->rotate_y(-dx * 0.01f);
                }
            }
        }

        glm::vec3 cam_Vel = glm::vec3(0.0f, 0.0f, 0.0f);
        if (get_keystate(SDL_SCANCODE_W)) {
            cam_Vel += camera->get_direction();
        }
        if (get_keystate(SDL_SCANCODE_S)) {
            cam_Vel -= camera->get_direction();
        }
        if (get_keystate(SDL_SCANCODE_A)) {
            cam_Vel -= camera->get_right();
        }
        if (get_keystate(SDL_SCANCODE_D)) {
            cam_Vel += camera->get_right();
        }

        if (glm::length(cam_Vel) > 0.0f) {
            cam_Vel = glm::normalize(cam_Vel) * deltaT;
            camera->translate(cam_Vel);
        }

        if (get_keystate(SDL_SCANCODE_C)) {
            mouse_captured = !mouse_captured;
        }

        camera->update(deltaT);

        renderer->render();

        const float end = SDL_GetTicks();
        deltaT = (end - start) / 1000.0f;
    }

    // Shutdown
    delete window;

    spdlog::info("Phosphor shut down");

}
