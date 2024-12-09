#pragma once

/**
 * Phosphor phosphor/light/directional.h
 * Directional light class
 */

#include <glm/glm.hpp>

#include "phosphor/light/light.h"

class DirectionalLight : public Light{
public:

    LightUniform lightParams;
    /**
     * Constructor
     */
    DirectionalLight(glm::vec3 direction, glm::vec3 color, float intensity);

    /**
     * Destructor
     */
    ~DirectionalLight() {}
};
