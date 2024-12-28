#pragma once

/**
* Phosphor include/phosphor/material.hpp
* Material class
*/

#include <string>

#include <glm/glm.hpp>

class Material {
protected:
    std::string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
public:
    Material() = default;
    Material(std::string name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) : name(name), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}
    ~Material() = default;

    std::string getName() { return name; }
    glm::vec3 getAmbient() { return ambient; }
    glm::vec3 getDiffuse() { return diffuse; }
    glm::vec3 getSpecular() { return specular; }
    float getShininess() { return shininess; }
};