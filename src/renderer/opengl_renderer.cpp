/**
 * Phosphor src/renderer/opengl_renderer.cpp
 * OpenGL renderer backend implementation.
 */

#include <string>

#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>

#include "phosphor/backends/opengl_renderer.hpp"

#include <phosphor/camera.hpp>

OpenGLRenderer::~OpenGLRenderer() {
    if (this->window != nullptr) {
        SDL_GL_DeleteContext(this->context);
    }
}

unsigned int vao, vbo;

Camera* camera;

int OpenGLRenderer::init() {
    //Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    this->context = SDL_GL_CreateContext(this->window);
    if (this->context == nullptr) {
        std::string error = SDL_GetError();
        spdlog::critical("Failed to create OpenGL context: {}", error);
        return RENDERER_ERROR;
    }

    //Initialize GLEW
    glewExperimental = GL_TRUE;
    if (const GLenum glewError = glewInit(); glewError != GLEW_OK) {
        std::string error = reinterpret_cast<const char*>(glewGetErrorString(glewError));
        spdlog::critical("Failed to initialize GLEW: {}", error);
        return RENDERER_ERROR;
    }

    spdlog::info("OpenGL context created successfully.");

    camera = new Camera(800, 600);

    return RENDERER_SUCCESS;
}

void OpenGLRenderer::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->currentScene->render();

    SDL_GL_SwapWindow(this->window);
}