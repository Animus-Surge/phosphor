/**
* Phosphor src/mesh.cpp
* Mesh implementation
*/

#include "phosphor/mesh/mesh.hpp"
#include "phosphor/util.hpp"

#include <GL/glew.h>
#include <phosphor/shader.hpp>
#include <spdlog/spdlog.h>

Mesh::Mesh(const std::string& filename) {
    //TODO: Implement mesh loading
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    //static std::shared_ptr<Shader> model_shader = std::make_shared<Shader>("resources/test.vert", "resources/test.frag");

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &this->IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &this->UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, this->UBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
}

Mesh *create_primitive_cube(float width, float height, float depth) {
    const std::vector<Vertex> vertices = {
        //Front face
        {{-width, -height, depth}},
        {{width, -height, depth}},
        {{width, height, depth}},
        {{-width, height, depth}},

        //Right face
        {{width, -height, depth}},
        {{width, -height, -depth}},
        {{width, height, -depth}},
        {{width, height, depth}},

        //Top face
        {{-width, height, depth}},
        {{width, height, depth}},
        {{width, height, -depth}},
        {{-width, height, -depth}},

        //Back face
        {{width, -height, -depth}},
        {{-width, -height, -depth}},
        {{-width, height, -depth}},
        {{width, height, -depth}},

        //Left face
        {{-width, -height, -depth}},
        {{-width, -height, depth}},
        {{-width, height, depth}},
        {{-width, height, -depth}},

        //Bottom face
        {{-width, -height, -depth}},
        {{width, -height, -depth}},
        {{width, -height, depth}},
        {{-width, -height, depth}}
    };

    const std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0, //Front face
        4, 5, 6, 6, 7, 4, //Right face
        8, 9, 10, 10, 11, 8, //Top face
        12, 13, 14, 14, 15, 12, //Back face
        16, 17, 18, 18, 19, 16, //Left face
        20, 21, 22, 22, 23, 20 //Bottom face
    };

    return new Mesh(vertices, indices);
}

Mesh *create_primitive_sphere(float radius) {
    return nullptr; //TODO: Implement sphere creation
}

Mesh *create_primitive_quad(float width, float height) {
    const std::vector<Vertex> vertices = {
        {{-width, -height, 0.0f}},
        {{width, -height, 0.0f}},
        {{width, height, 0.0f}},
        {{-width, height, 0.0f}}
    };

    const std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};

    return new Mesh(vertices, indices);
}

void Mesh::render() {
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
