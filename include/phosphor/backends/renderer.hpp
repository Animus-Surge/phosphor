#pragma once

/**
 * Phosphor include/phosphor/backends/renderer.hpp
 * Renderer class for Phosphor.
 */

#include <SDL2/SDL.h>

#include "phosphor/scene.hpp"

/**
 * Status codes for Renderer operations.
 */

enum RendererStatus {
    RENDERER_SUCCESS = 0,
    RENDERER_ERROR = 1,
    RENDERER_VULKAN_NOT_SUPPORTED = 2
};

/**
 * Base Renderer class for Phosphor.
 */
class Renderer {
protected:
    SDL_Window* window = nullptr;
public:
    Scene* currentScene;

    virtual int init() = 0;
    virtual void render() = 0;

    void set_window(SDL_Window* window) {
        this->window = window;
    }

    void set_scene(Scene* scene) {
        this->currentScene = scene;
    }
    [[nodiscard]] Scene* get_scene() const {
        return this->currentScene;
    }

};

