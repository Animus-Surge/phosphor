/**
 * Phosphor mesh/cube.cpp
 * Cube mesh implementation
 */

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "phosphor/mesh/cube.hpp"

Cube::Cube() {

    this->material = new Material(); //TODO: add material constructor with data

    //Generate vertices
    this->vertices = {
        //Front face
        { glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.0, 0.0, -1.0)},
        { glm::vec3(0.5, -0.5, -0.5), glm::vec3(0.0, 0.0, -1.0)},
        { glm::vec3(0.5, 0.5, -0.5), glm::vec3(0.0, 0.0, -1.0)},
        { glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0.0, 0.0, -1.0)},

        //Right face
        { glm::vec3(0.5, -0.5, -0.5), glm::vec3(1.0, 0.0, 0.0)},
        { glm::vec3(0.5, -0.5, 0.5), glm::vec3(1.0, 0.0, 0.0)},
        { glm::vec3(0.5, 0.5, 0.5), glm::vec3(1.0, 0.0, 0.0)},
        { glm::vec3(0.5, 0.5, -0.5), glm::vec3(1.0, 0.0, 0.0)},

        //Top face
        { glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0.0, 1.0, 0.0)},
        { glm::vec3(0.5, 0.5, -0.5), glm::vec3(0.0, 1.0, 0.0)},
        { glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.0, 1.0, 0.0)},
        { glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0.0, 1.0, 0.0)},

        //Back face
        { glm::vec3(0.5, -0.5, 0.5), glm::vec3(0.0, 0.0, 1.0)},
        { glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0.0, 0.0, 1.0)},
        { glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0.0, 0.0, 1.0)},
        { glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.0, 0.0, 1.0)},

        //Left face
        { glm::vec3(-0.5, -0.5, 0.5), glm::vec3(-1.0, 0.0, 0.0)},
        { glm::vec3(-0.5, -0.5, -0.5), glm::vec3(-1.0, 0.0, 0.0)},
        { glm::vec3(-0.5, 0.5, -0.5), glm::vec3(-1.0, 0.0, 0.0)},
        { glm::vec3(-0.5, 0.5, 0.5), glm::vec3(-1.0, 0.0, 0.0)},

        //Bottom face
        { glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0.0, -1.0, 0.0)},
        { glm::vec3(0.5, -0.5, 0.5), glm::vec3(0.0, -1.0, 0.0)},
        { glm::vec3(0.5, -0.5, -0.5), glm::vec3(0.0, -1.0, 0.0)},
        { glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.0, -1.0, 0.0)}
    };

    this->indices = {
        0, 3, 2, 2, 1, 0, //Front face
        4, 7, 6, 6, 5, 4, //Right face
        8, 11, 10, 10, 9, 8, //Top face
        12, 15, 14, 14, 13, 12, //Back face
        16, 19, 18, 18, 17, 16, //Left face
        20, 23, 22, 22, 21, 20 //Bottom face
    };

    //Generate vertex array object
    glGenVertexArrays(1, &this->vertex_array);
    glBindVertexArray(this->vertex_array);

    //Generate vertex buffer object
    glGenBuffers(1, &this->vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW);

    //Generate index buffer object
    glGenBuffers(1, &this->index_buffer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);

    //Generate uniform buffer object
    glGenBuffers(1, &this->uniform_buffer_id);
    glBindBuffer(GL_UNIFORM_BUFFER, this->uniform_buffer_id);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), &this->transform, GL_DYNAMIC_DRAW);

    //Set up vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
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
    this->material->bind();
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->uniform_buffer_id);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Cube::regen_ubo() {
    //Regenerate uniform buffer object
    glBindBuffer(GL_UNIFORM_BUFFER, this->uniform_buffer_id);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), &this->transform, GL_DYNAMIC_DRAW);
}

void Cube::set_transform(glm::mat4 transform) {
    this->transform = transform;
    this->regen_ubo();
}
