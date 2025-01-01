#pragma once

/**
* Phosphor include/phosphor/texture.hpp
* Texture class
*/

#include <string>

#include <GL/glew.h>
#include <spdlog/spdlog.h>

class Texture {
protected:
    unsigned int id = 0;
    int width, height, channels;

public:
    Texture(const unsigned char* data, const int width, const int height, const int channels):
        width(width), height(height), channels(channels) {
        if (!data) {
            spdlog::error("Texture not created; no data present");
            return;
        }

        //Generate texture
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        //Format selection
        int texture_format = GL_RGB;
        if (channels == 4) {
            texture_format = GL_RGBA;
        }

        //Parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        //Bind data
        glTexImage2D(GL_TEXTURE_2D, 0, texture_format, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    ~Texture() {
        glDeleteTextures(1, &id);
    }

    void bind(const int slot = 0) const {
        glBindTexture(GL_TEXTURE_2D, id);
        glActiveTexture(GL_TEXTURE0 + slot);

    }
    void unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    [[nodiscard]] int get_width() const { return width; }
    [[nodiscard]] int get_height() const { return height; }
    [[nodiscard]] int get_channels() const { return channels; }
};
