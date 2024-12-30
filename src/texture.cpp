/**
* Phosphor src/texture.cpp
* Texture class implementation
*/

#include "phosphor/texture.hpp"

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, id);
}
