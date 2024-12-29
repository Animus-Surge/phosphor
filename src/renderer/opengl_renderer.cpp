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
#include "phosphor/shader.hpp"
#include "phosphor/camera.hpp"
#include "phosphor/mesh/mesh.hpp"

OpenGLRenderer::~OpenGLRenderer() {
    if (this->window != nullptr) {
        SDL_GL_DeleteContext(this->context);
    }
}

unsigned int vao, vbo, ibo, ubo;
Camera* camera;
Shader* shader;

std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}},
    {{0.5f, 0.5f, 0.0f}},
    {{-0.5f, 0.5f, 0.0f}}
};
std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};

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
    shader = new Shader("resources/test.vert", "resources/test.frag");

    constexpr auto transform = glm::mat4(1.0f);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), &transform, GL_DYNAMIC_DRAW);

    glCullFace(GL_FALSE);
    // glEnable(GL_DEPTH_TEST);
    return RENDERER_SUCCESS;
}

void OpenGLRenderer::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();
    glBindVertexArray(vao);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    SDL_GL_SwapWindow(this->window);
}