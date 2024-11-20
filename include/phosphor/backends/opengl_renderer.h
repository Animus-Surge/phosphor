#pragma once

/**
 * Phosphor phosphor/backends/opengl_renderer.h
 * OpenGL renderer system
 */

#include "phosphor/renderer.h"

class OpenGLRenderer : public Renderer {
public:
    void init() override;
    void shutdown() override;
    void render() override;
}; // class OpenGLRenderer

