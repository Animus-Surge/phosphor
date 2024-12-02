/**
 * Phosphor mesh/cube.cpp
 * Cube mesh implementation
 */

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "phosphor/mesh/cube.h"

Cube::Cube() {
    //Generate cube vertices
    //TODO: global space coordinate input

    glm::vec3 vertices[] = {
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3( 0.5f, -0.5f,  0.5f),
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f)
    };

    //Generate cube indices
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
        1, 5, 6,
        6, 2, 1,
        7, 6, 5,
        5, 4, 7,
        4, 0, 3,
        3, 7, 4,
        4, 5, 1,
        1, 0, 4,
        3, 2, 6,
        6, 7, 3
    };
    
    //Generate and bind VAO
    glGenVertexArrays(1, &this->vertex_array);
    glBindVertexArray(this->vertex_array);

    //Generate and bind VBO
    glGenBuffers(1, &this->vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Generate and bind IBO
    glGenBuffers(1, &this->index_buffer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Uniform buffer
    glGenBuffers(1, &this->uniform_buffer_id);
    glBindBuffer(GL_UNIFORM_BUFFER, this->uniform_buffer_id);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), &this->transform, GL_DYNAMIC_DRAW);
}

Cube::~Cube() {
    glDisableVertexAttribArray(0);
    glDeleteVertexArrays(1, &this->vertex_array);
    glDeleteBuffers(1, &this->vertex_buffer_id);
    glDeleteBuffers(1, &this->index_buffer_id);
    glDeleteBuffers(1, &this->uniform_buffer_id);
}

void Cube::render() {
    glBindVertexArray(this->vertex_array);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->uniform_buffer_id);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Cube::set_transform(glm::mat4 transform) {
    this->transform = transform;

    //Update uniform buffer
    glBindBuffer(GL_UNIFORM_BUFFER, this->uniform_buffer_id);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), &this->transform, GL_DYNAMIC_DRAW);
}
