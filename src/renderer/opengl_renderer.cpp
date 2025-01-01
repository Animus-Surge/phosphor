/**
 * Phosphor src/renderer/opengl_renderer.cpp
 * OpenGL renderer backend implementation.
 */

#include <string>

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>

#include "phosphor/backends/opengl_renderer.hpp"
#include "phosphor/imgui.hpp"
#include "phosphor/shader.hpp"
#include "phosphor/camera.hpp"
#include "phosphor/mesh/mesh.hpp"
#include "phosphor/mesh/axis_widget.hpp"

OpenGLRenderer::~OpenGLRenderer() {
    if (this->window != nullptr) {
        SDL_GL_DeleteContext(this->context);
    }
}

unsigned int vao, vbo, ibo, ubo;
Camera* camera;
Shader* shader;
Mesh* mesh;
AxisWidget* axis_widget;

std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}},
    {{0.5f, 0.5f, 0.0f}},
    {{-0.5f, 0.5f, 0.0f}}
};
std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};

void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userparam) {
    if (message == nullptr) {
        spdlog::error("GL CALLBACK: Received null message");
        return;
    }

    if (type == GL_DEBUG_TYPE_ERROR) {
        spdlog::error("GL ERROR[{}] - {}: {}", severity, id, message);
    } else {
        spdlog::info("GL CALLBACK[{}] {} - {}: {}", severity, type, id, message);
    }
}

int OpenGLRenderer::init() {
    //Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);


    this->context = SDL_GL_CreateContext(this->window);
    if (this->context == nullptr) {
        std::string error = SDL_GetError();
        spdlog::critical("Failed to create OpenGL context: {}", error);
        return RENDERER_ERROR;
    }

    if (glDebugMessageCallback != nullptr) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(messageCallback, nullptr);
    } else {
        spdlog::warn("glDebugMessageCallback not available.");
    }

    //Initialize GLEW
    glewExperimental = GL_TRUE;
    if (const GLenum glewError = glewInit(); glewError != GLEW_OK) {
        std::string error = reinterpret_cast<const char*>(glewGetErrorString(glewError));
        spdlog::critical("Failed to initialize GLEW: {}", error);
        return RENDERER_ERROR;
    }

    imgui_init(this->window, this->context);

    spdlog::info("OpenGL context created successfully.");

    // glCullFace(GL_FALSE);
    glEnable(GL_DEPTH_TEST);

    axis_widget = new AxisWidget();

    return RENDERER_SUCCESS;
}

void OpenGLRenderer::render() {
    //ImGui
    imgui_newFrame();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    currentScene->render();
    axis_widget->render();

    imgui_render();

    SDL_GL_SwapWindow(this->window);
}