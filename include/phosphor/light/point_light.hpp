#pragma once

/**
 * Phosphor phosphor/light/point_light.hpp
 * Point light class
 */

#include <glm/glm.hpp>

#include "phosphor/light/light.hpp"

class PointLight : public Light {
public:
    PointLight() {}
    ~PointLight() {
        glDeleteBuffers(1, &this->light_ubo);
    }

    void set_position(glm::vec3 position) {
        this->lightData.light_position = position;
    }
    void set_radius(float radius) {
        this->lightData.light_radius = radius;
    }
    void set_constant_attenuation(float attenuation) {
        this->lightData.light_constant_attenuation = attenuation;
    }
    void set_linear_attenuation(float attenuation) {
        this->lightData.light_linear_attenuation = attenuation;
    }
    void set_quadratic_attenuation(float attenuation) {
        this->lightData.light_quadratic_attenuation = attenuation;
    }

    glm::vec3 get_position() {
        return this->lightData.light_position;
    }
    float get_radius() {
        return this->lightData.light_radius;
    }
    float get_constant_attenuation() {
        return this->lightData.light_constant_attenuation;
    }
    float get_linear_attenuation() {
        return this->lightData.light_linear_attenuation;
    }
    float get_quadratic_attenuation() {
        return this->lightData.light_quadratic_attenuation;
    }

    void gen_ubo() {
        glGenBuffers(1, &this->light_ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, this->light_ubo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(LightData), &this->lightData, GL_DYNAMIC_DRAW);
    }
}; // class PointLight
