#pragma once

/**
 * Phosphor phosphor/core/material.hpp
 * Material class
 */

#include <string>

#include <glm/glm.hpp>

struct MaterialData {
    glm::vec4 albedo = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 emissive = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    float roughness = 0.5f;
    float specular = 0.5f;
    //TODO: add more material properties
};

class Material {
protected:
    MaterialData data;
    unsigned int ubo;

    //TODO: editor gui, textures, etc.

public:
    Material() {
        data = MaterialData();
        gen_ubo();
    }
    Material(MaterialData data) : data(data) {
        gen_ubo();
    }

    MaterialData get_data() { return data; }
    void set_data(MaterialData data) { this->data = data; }

    void set_albedo(glm::vec4 albedo) { data.albedo = albedo; }
    void set_emissive(glm::vec4 emissive) { data.emissive = emissive; }
    void set_roughness(float roughness) { data.roughness = roughness; }
    void set_specular(float specular) { data.specular = specular; }

    glm::vec4 get_albedo() { return data.albedo; }
    glm::vec4 get_emissive() { return data.emissive; }
    float get_roughness() { return data.roughness; }
    float get_specular() { return data.specular; }

    void gen_ubo();

    void __editor_gui__() { //TODO
    }

    void bind();

};
