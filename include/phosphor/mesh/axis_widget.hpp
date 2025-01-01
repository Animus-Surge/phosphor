#pragma once

/**
 * Phosphor include/phosphor/mesh/axis_widget.hpp
 * Axis widget class
 */

#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "phosphor/shader.hpp"

const std::string axis_vert = R"(
#version 450

layout(binding = 0) uniform TransformData {
    mat4 transform;
};
layout(binding = 1) uniform CameraData {
    mat4 view;
    mat4 projection;
    mat4 view_projection;
    vec3 position;
    vec3 direction;
    vec3 right;
    vec3 up;
};

layout(location = 0) in vec3 vert_position;
layout(location = 1) in vec3 vert_color;

out vec3 frag_color;

void main() {
    gl_Position = view_projection * transform * vec4(vert_position, 1.0);
    frag_color = vert_color;
}
)";

const std::string axis_frag = R"(
#version 450

in vec3 frag_color;

out vec4 FragColor;

void main() {
    FragColor = vec4(frag_color, 1.0);
}
)";

class AxisWidget {
    unsigned int VAO = 0, VBO = 0, IBO = 0, UBO = 0;
    glm::mat4 transform = glm::mat4(1.0f);
    std::vector<float> vertices = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
        };
    std::vector<unsigned int> indices = {0, 1, 2, 3, 4, 5};

    Shader* shader = new Shader(axis_vert, axis_frag, false);

public:
    AxisWidget() {
        glGenVertexArrays(1, &this->VAO);
        glBindVertexArray(this->VAO);

        glGenBuffers(1, &this->VBO);
        glBindBuffer(GL_VERTEX_ARRAY, this->VBO);
        glBufferData(GL_VERTEX_ARRAY, this->vertices.size() * sizeof(float), this->vertices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &UBO);
        glBindBuffer(GL_UNIFORM_BUFFER, this->UBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), &this->transform, GL_DYNAMIC_DRAW);
    }
    ~AxisWidget() {
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);
        glDeleteBuffers(1, &this->IBO);
        glDeleteBuffers(1, &this->UBO);
    }

    void render() const {
        this->shader->use();
        glBindVertexArray(VAO);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->UBO);
        glDrawElements(GL_LINES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
        this->shader->drop();
        glBindVertexArray(0);
    }
};