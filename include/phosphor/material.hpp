#pragma once

/**
* Phosphor include/phosphor/material.hpp
* Material class
*/

#include <glm/glm.hpp>

#include "texture.hpp"

struct MaterialData {

    glm::vec3 albedo = glm::vec3(1.0f);
    glm::vec3 diffuse = glm::vec3(1.0f);
    glm::vec3 specular = glm::vec3(1.0f);

    float metallic = 0.5f;
    float roughness = 0.5f;

    float has_texture = 0.0f; // 0.0f = false, 1.0f = true; used in the shader to determine if a texture is present

    float padding;
};

class Material {
protected:
    unsigned int ubo_id;

    MaterialData data;

    Texture* texture = nullptr;

    void gen_ubo() {
        glGenBuffers(1, &ubo_id);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo_id);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialData), &data, GL_DYNAMIC_DRAW);
    }

    void regen_ubo() const {
        glBindBuffer(GL_UNIFORM_BUFFER, ubo_id);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(MaterialData), &data);
    }
public:
    Material() : ubo_id(0)  {
        data = {
            glm::vec3(1.0f),
        };
        gen_ubo();
    }

    [[nodiscard]] glm::vec3 get_albedo() const {
        return data.albedo;
    }

    [[nodiscard]] glm::vec3 get_diffuse() const {
        return data.diffuse;
    }

    [[nodiscard]] float get_metallic() const {
        return data.metallic;
    }

    [[nodiscard]] float get_roughness() const {
        return data.roughness;
    }

    void set_albedo(const glm::vec3 newAlbedo) {
        data.albedo = newAlbedo;
        regen_ubo();
    }

    void set_diffuse(const glm::vec3 newDiffuse) {
        data.diffuse = newDiffuse;
        regen_ubo();
    }

    void set_specular(const glm::vec3 newSpecular) {
        data.specular = newSpecular;
        regen_ubo();
    }

    void set_metallic(const float newMetallic) {
        data.metallic = newMetallic;
        regen_ubo();
    }

    void set_roughness(const float newRoughness) {
        data.roughness = newRoughness;
        regen_ubo();
    }

    void set_texture(Texture* texture) {
        this->texture = texture;
        data.has_texture = 1.0f;
        regen_ubo();
    }
    void remove_texture() {
        this->texture = nullptr;
        data.has_texture = 0.0f;
        regen_ubo();
    }

    void bind() const {
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, ubo_id);
    }
};