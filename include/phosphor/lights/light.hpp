#pragma once

/**
 * Phosphor include/phosphor/lights/light.hpp
 * Light class
 */

#include <glm/glm.hpp>

#include "phosphor/object.hpp"

struct PointLightData {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 1.0f;
    float radius = 1.0f; //aka falloff

    float padding[2];
};

class Light : public Component {
    unsigned int ubo = 0;

    PointLightData data;

public:
    Light() {
        glGenBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);

        data = {
            glm::vec3(0.0f),
            glm::vec3(1.0f),
            1.0f,
            1.0f
        };
    }
    ~Light() override {
        glDeleteBuffers(1, &ubo);
    }

    //Mutators
    void set_color(const glm::vec3 color) {
        data.color = color;
    }
    void set_intensity(const float intensity) {
        data.intensity = intensity;
    }
    void set_position(const glm::vec3 position) {
        data.position = position;
    }
    void set_radius(const float radius) {
        data.radius = radius;
    }

    void update() override { //Update light uniform
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PointLightData), &data);
    }
    void fixedUpdate(float dt) override {}
    void render() override { //Bind light uniform
        glBindBufferBase(GL_UNIFORM_BUFFER, 3, ubo);
    }
};