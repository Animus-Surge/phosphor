#pragma once

/**
 * Phosphor phosphor/backends/opengl_renderer.h
 * OpenGL renderer system
 */

#include "phosphor/renderer.h"

class OpenGLRenderer : public Renderer {
protected:
    SDL_GLContext context;        // OpenGL context
public:
    void init() override;
    void shutdown() override;
    void run() override;
}; // class OpenGLRenderer

