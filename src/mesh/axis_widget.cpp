/**
 * Phosphor mesh/axis_widget.cpp
 * Axis widget mesh implementation
 */

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "phosphor/mesh/axis_widget.h"

AxisWidget::AxisWidget() {
    this->axis_shader = new Shader("resources/axis_widget_vert.glsl", "resources/axis_widget_frag.glsl");

    float vertices[] = {
    //  Position            Color
        0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f, // X axis origin
        1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f, 

        0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f, // Y axis origin
        0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f, // Z axis origin
        0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f
    };

    glGenVertexArrays(1, &this->vertex_array);
    glBindVertexArray(this->vertex_array);

    glGenBuffers(1, &this->vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenBuffers(1, &this->uniform_buffer_id);
    glBindBuffer(GL_UNIFORM_BUFFER, this->uniform_buffer_id);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), &this->transform, GL_DYNAMIC_DRAW);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->uniform_buffer_id);
}
AxisWidget::~AxisWidget() {
    glDeleteBuffers(1, &this->vertex_buffer_id);
    glDeleteVertexArrays(1, &this->vertex_array);
    delete this->axis_shader;
}

void AxisWidget::render() {
    this->axis_shader->use();
    glBindVertexArray(this->vertex_array);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->uniform_buffer_id);
    glDrawArrays(GL_LINES, 0, 6);
    this->axis_shader->drop();
    glBindVertexArray(0);
}

