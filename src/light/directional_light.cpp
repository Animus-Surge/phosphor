/**
 * Phosphor light/directional_light.cpp
 * Directional light implementation
 */

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "phosphor/light/directional.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color, float intensity) {
    this->lightParams.direction = direction;
    this->lightParams.color = color;
    this->lightParams.intensity = intensity;
//    this->lightParams.position = glm::vec3(0.0f, 0.0f, 0.0f);

    glGenBuffers(1, &uniform_buffer_id);
    glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_id);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(LightUniform), &lightParams, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, uniform_buffer_id);
}
