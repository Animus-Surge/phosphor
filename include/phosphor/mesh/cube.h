#pragma once

/**
 * Phosphor phosphor/cube.h
 * Cube mesh class
 */

#include "phosphor/mesh/mesh.h"

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

    void set_transform(glm::mat4 transform);
}; // class Cube
