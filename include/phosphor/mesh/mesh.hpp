#pragma once

/**
 * Phosphor include/phosphor/mesh/mesh.hpp
 * Base mesh class
 */

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>

#include "phosphor/object.hpp"
#include "phosphor/material.hpp"
#include "phosphor/shader.hpp"

/**
 * Vertex structure containing data points required for the mesh shader
 */
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

/**
 * Mesh class representing a 3D object
 */
class Mesh final : public Component {
    static Material* defaultMaterial;
    static Shader* mesh_shader;
    static void init_mesh_shader() {
        if (mesh_shader == nullptr) {
            mesh_shader = new Shader("resources/mesh_common.vert", "resources/mesh_common.frag");
        }
    }


protected:
    unsigned int VAO = 0, VBO = 0, IBO = 0, UBO = 0;
    glm::mat4 transform = glm::mat4(1.0f);
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Material* material = nullptr;
public:
    void regen_ubo() const {
        glBindBuffer(GL_UNIFORM_BUFFER, this->UBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &this->transform);
    }

    void gen_buffers() {

        //Gen vertex array and buffers
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->IBO);
        glGenBuffers(1, &this->UBO);

        //Binding and setting the data
        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_UNIFORM_BUFFER, this->UBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), &this->transform, GL_DYNAMIC_DRAW);

        //Set vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
        glEnableVertexAttribArray(2);

        //Unbind vertex array
        glBindVertexArray(0);
    }

    //TODO: Material, model loading

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) : vertices(vertices), indices(indices) {
        init_mesh_shader();
        gen_buffers();
    }

    ~Mesh() override {
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);
        glDeleteBuffers(1, &this->IBO);
        glDeleteBuffers(1, &this->UBO);
    }

    void translate(const glm::vec3 delta) {
        this->transform = glm::translate(this->transform, delta);
        regen_ubo();
    }

    void render() override {
        if (!material) { //If no material is set, use the default material
            material = defaultMaterial;
        }
        mesh_shader->use();
        material->bind();
        glBindVertexArray(this->VAO);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->UBO);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
        mesh_shader->drop();
        glBindVertexArray(0);
    }

    static void set_default_material(Material* newMaterial) {
        defaultMaterial = newMaterial;
    }
};

/*
 * 2D shapes
 */

/**
 * \brief Creates a 2D quad
 * @param width Width of the quad
 * @param height Height of the quad
 * @return A Mesh object representing the quad
 */
Mesh* create_primitive_quad(float width, float height);

/**
 * \brief Creates a 2D circle
 * @param radius The radius of the circle
 * @return A mesh object representing the circle
 */
Mesh* create_primitive_circle(float radius);

/**
 * \brief Creates a 2D line
 * @param start Start point of the line
 * @param end End point of the line
 * @return A Mesh object representing the line
 */
Mesh* create_primitive_line(glm::vec2 start, glm::vec2 end);

/*
 * 3D shapes
 */

/**
 * \brief Creates a 3D box
 * @param width Size of the box in the x direction
 * @param height Size of the box in the y direction
 * @param depth Size of the box in the z direction
 * @return A Mesh object representing the box
 */
Mesh* create_primitive_box(float width, float height, float depth);

/**
 * \brief Creates a 3D sphere
 * @param radius Radius of the sphere
 * @param slices Number of slices
 * @param stacks Number of stacks
 * @return A Mesh object representing the sphere
 */
Mesh* create_primitive_sphere(float radius, int slices, int stacks);

/**
 * \brief Creates a 3D cylinder
 * @param radius Radius of the cylinder
 * @param height Height of the cylinder
 * @param slices Number of slices
 * @return A Mesh object representing the cylinder
 */
Mesh* create_primitive_cylinder(float radius, float height, int slices);

