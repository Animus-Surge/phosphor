#pragma once

/**
 * Phosphor phosphor/cube.h
 * Cube mesh class
 */

#include <glm/glm.hpp>

#include "phosphor/mesh/mesh.hpp"

class Cube : public Mesh {
public:
    /**
     * Constructor
     */
    Cube();

    /**
     * Destructor
     */
    ~Cube();

    /**
     * Render the cube
     */
    void render() override;

    /**
     * Regenerate the uniform buffer object
     */
    void regen_ubo() override;

    void set_transform(glm::mat4 transform);
}; // class Cube
