#pragma once

/**
 * Phosphor phosphor/light/light.h
 * Base light class
 */

#include <glm/glm.hpp>

struct LightUniform {
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    float falloff;
    float padding[2];
};

class Light {
protected:
    unsigned int uniform_buffer_id = -1;


public:
    /**
     * Constructor
     */
    Light() {}

    /**
     * Destructor
     */
    virtual ~Light() {}

    /**
     * Bind the light
     */
    void bind() {
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, uniform_buffer_id);
    }
}; // class Light
