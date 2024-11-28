#pragma once

/**
 * Phosphor phosphor/mesh/mesh.h
 * Base mesh class
 */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Mesh {
protected:
    /**
     * Vertex array object ID
     */
    unsigned int vertex_array;
    
    /**
     * Vertex buffer object ID
     */
    unsigned int vertex_buffer_id;
    
    /**
     * Index buffer object ID
     */
    unsigned int index_buffer_id;

    /**
     * Uniform buffer object ID
     */
    unsigned int uniform_buffer_id;

    /**
     * Transform matrix
     */
    glm::mat4 transform = glm::mat4(1.0f);
    
public:
    /**
     * Constructor
     */
    Mesh() {}

    /**
     * Destructor
     */
    ~Mesh() {}

    /**
     * Render the mesh
     */
    virtual void render() = 0; //Using pure virtual function to make this class abstract
    
    unsigned int get_vbo() { return vertex_buffer_id; }
    unsigned int get_ibo() { return index_buffer_id; }
    unsigned int get_ubo() { return uniform_buffer_id; }

    void set_transform(glm::mat4 transform) { this->transform = transform; }
    glm::mat4 get_transform() { return this->transform; }

    void translate(glm::vec3 translation) { this->transform = glm::translate(this->transform, translation); }
    void rotate(float angle, glm::vec3 axis) { this->transform = glm::rotate(this->transform, angle, axis); }
    void scale(glm::vec3 scale) { this->transform = glm::scale(this->transform, scale); }
}; // class Mesh
