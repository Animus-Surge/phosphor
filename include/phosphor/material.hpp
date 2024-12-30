#pragma once

/**
* Phosphor include/phosphor/material.hpp
* Material class
*/

#include <glm/glm.hpp>

#include "texture.hpp"

struct MaterialData {
    glm::vec3 albedo;
    float metallic;
    float roughness;
};

class Material {
protected:
    glm::vec3 albedo;
    float metallic;
    float roughness;

    Texture* texture = nullptr;

    //TODO: Texture support, more material properties
public:
    Material() = default;
    ~Material() = default;
};