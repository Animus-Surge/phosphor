/**
 * Phosphor phosphor/material.cpp
 * Material class implementation
 */

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "phosphor/core/material.hpp"

void Material::gen_ubo() {
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialData), &data, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Material::bind() {
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->ubo);
}
