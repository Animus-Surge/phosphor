#pragma once

/**
 * Phosphor phosphor/light/light.h
 * Light system and declarations
 */

#include <glm/glm.hpp>

#include "phosphor/core/component.hpp"

struct LightData {
    int type = 0; //0 = point, 1 = directional, 2 = spot, ...
    // Common light data
    glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
    float light_intensity = 0.5f;

    // Point light data
    glm::vec3 light_position = glm::vec3(0.0f, 0.0f, 0.0f);
    float light_radius = 10.0f;
    float light_constant_attenuation = 1.0f;
    float light_linear_attenuation = 0.09f;
    float light_quadratic_attenuation = 0.032f;

    // Directional light data
    glm::vec3 light_direction = glm::vec3(0.0f, 0.0f, 0.0f); //Also used with spotlights

    // Spot light data
    float light_cutoff = 0.0f;
    float light_outer_cutoff = 0.0f;

    //TODO: shadows
};

class Light : public Component {
protected:
    LightData lightData;
    unsigned int light_ubo;

public:
    Light() {}
    ~Light() {
        glDeleteBuffers(1, &this->light_ubo);
    }

    void init() override {}
    void update() override {}
    void fixedUpdate(float delta) override {}
    void render() override {
        this->bind();
    }
    void shutdown() override {}

    void set_color(glm::vec3 color) {
        this->lightData.light_color = color;
    }
    void set_intensity(float intensity) {
        this->lightData.light_intensity = intensity;
    }

    glm::vec3 get_color() {
        return this->lightData.light_color;
    }
    float get_intensity() {
        return this->lightData.light_intensity;
    }

    virtual void gen_ubo() = 0;

    void bind() {
        //Rebind data
        glBindBuffer(GL_UNIFORM_BUFFER, this->light_ubo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(LightData), &this->lightData, GL_DYNAMIC_DRAW);

        //Bind UBO to position 3
        glBindBufferBase(GL_UNIFORM_BUFFER, 3, this->light_ubo);
    }

};
