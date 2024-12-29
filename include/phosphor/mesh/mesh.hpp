#pragma once

/**
 * Phosphor include/phosphor/mesh/mesh.hpp
 * Base mesh class
 */

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "phosphor/object.hpp"
#include "phosphor/material.hpp"

struct Vertex {
    glm::vec3 position;
};

class Mesh final : public Component {
protected:
    unsigned int VAO = 0, VBO = 0, IBO = 0, UBO = 0;
    glm::mat4 transform;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Material* material = nullptr;
public:
    /**
     * \brief Construct a mesh from a file
     * @param filename file to load mesh from
     */
    Mesh(const std::string& filename);

    /**
     * \brief Construct a mesh from vertices and indices
     * @param vertices vertices of the mesh
     * @param indices indices of the mesh
     */
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

    ~Mesh() override {
        glDeleteBuffers(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);
        glDeleteBuffers(1, &this->IBO);
        glDeleteBuffers(1, &this->UBO);
    }

    //Accessors
    Material* getMaterial() const { return material; }

    //Mutators
    void setMaterial(Material* material) { this->material = material; }

    void translate(glm::vec3 translation);
    void rotate(float angle, glm::vec3 axis);
    void scale(glm::vec3 scale);

    std::vector<Vertex> get_vertices() const { return vertices; }
    std::vector<unsigned int> get_indices() const { return indices; }

    /**
     * \brief Render the mesh
     */
    void render() override;
};

Mesh* create_primitive_cube(float width, float height, float depth);
Mesh* create_primitive_sphere(float radius);
Mesh* create_primitive_quad(float width, float height);