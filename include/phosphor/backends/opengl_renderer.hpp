#pragma once

/**
 * Phosphor include/phosphor/backends/opengl_renderer.hpp
 * OpenGL renderer backend for Phosphor.
 */

#include "phosphor/backends/renderer.hpp"

#include <SDL2/SDL.h>

class OpenGLRenderer : public Renderer {
    protected:
        SDL_GLContext context;
    public:
        OpenGLRenderer() {}
        ~OpenGLRenderer();

        int init() override;
        void render() override;
};