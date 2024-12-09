/**
 * Phosphor mesh/worldgrid.cpp
 * World grid mesh implementation
 */

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "phosphor/mesh/worldgrid.h"
#include "phosphor/shader.h"

Shader* gridShader;

WorldGrid::WorldGrid(float grid_line_spacing, int grid_size) {
    this->grid_line_spacing = grid_line_spacing;
    this->grid_size = grid_size;

    gridShader = new Shader("./resources/grid_shader_vert.glsl", "./resources/grid_shader_frag.glsl");

    //Generate the grid
    glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f); //TODO: move this to a variable

    //Generate the grid lines
    
    // X axis
    for(int i = -grid_size; i <= grid_size; i++) {
        vertices.push_back({glm::vec3(i * grid_line_spacing, 0.0f, -grid_size * grid_line_spacing), glm::vec3(0.0f, 1.0f, 0.0f), color});
        vertices.push_back({glm::vec3(i * grid_line_spacing, 0.0f, grid_size * grid_line_spacing), glm::vec3(0.0f, 1.0f, 0.0f), color});
    }

    // Z axis
    for(int i = -grid_size; i <= grid_size; i++) {
        vertices.push_back({glm::vec3(-grid_size * grid_line_spacing, 0.0f, i * grid_line_spacing), glm::vec3(0.0f, 1.0f, 0.0f), color});
        vertices.push_back({glm::vec3(grid_size * grid_line_spacing, 0.0f, i * grid_line_spacing), glm::vec3(0.0f, 1.0f, 0.0f), color});
    }

    //Generate the index buffer
    for(int i = 0; i < vertices.size(); i++) {
        indices.push_back(i);
    }

    //Generate the vertex array object
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    //Generate the vertex buffer object
    glGenBuffers(1, &vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    //Generate the index buffer object
    glGenBuffers(1, &index_buffer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    //Set the vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(2);

    //Generate the uniform buffer object
    glGenBuffers(1, &uniform_buffer_id);
    glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_id);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffer_id);

    //Unbind the vertex array object
    glBindVertexArray(0);
}

WorldGrid::~WorldGrid() {}

void WorldGrid::render() {
    glBindVertexArray(vertex_array);
    gridShader->use();
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffer_id);
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    gridShader->drop();
}

