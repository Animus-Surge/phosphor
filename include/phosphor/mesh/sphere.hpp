#pragma once

/**
 * Phosphor phosphor/mesh/sphere.h
 * Sphere mesh declaration
 */

#include <vector>

#include <glm/glm.hpp>

#include "phosphor/mesh/mesh.hpp"

class Sphere : public Mesh {
protected:
    float radius = 1.0f;

public:
    Sphere();
    ~Sphere() {}

    void render() override;
};