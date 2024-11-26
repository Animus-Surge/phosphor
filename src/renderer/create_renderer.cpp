/**
 * Phosphor renderer/create_renderer.cpp
 * Renderer factory implementation
 */

#include "phosphor/renderer.h"
#include "phosphor/backends/opengl_renderer.h"

std::unique_ptr<Renderer> create_renderer(int type) {
    switch (type) {
        case 0:
            return std::make_unique<OpenGLRenderer>();
        default:
            return nullptr;
    }
}
