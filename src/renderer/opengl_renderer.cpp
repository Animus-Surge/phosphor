/**
 * Phosphor src/renderer/opengl_renderer.cpp
 * OpenGL renderer backend implementation.
 */

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <spdlog/spdlog.h>

#include "phosphor/backends/opengl_renderer.hpp"

OpenGLRenderer::~OpenGLRenderer() {
    if (this->window != nullptr) {
        SDL_GL_DeleteContext(this->context);
    }
}

int OpenGLRenderer::init() {
    //Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    this->context = SDL_GL_CreateContext(this->window);
    if (this->context == nullptr) {
        std::string error = SDL_GetError();
        spdlog::critical("Failed to create OpenGL context: {}", error);
        return RENDERER_ERROR;
    }

    //Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        std::string error = reinterpret_cast<const char*>(glewGetErrorString(glewError));
        spdlog::critical("Failed to initialize GLEW: {}", error);
        return RENDERER_ERROR;
    }

    spdlog::info("OpenGL context created successfully.");

    return RENDERER_SUCCESS;
}

void OpenGLRenderer::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //Scene rendering goes here...

    SDL_GL_SwapWindow(this->window);
}