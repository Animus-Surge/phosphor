#pragma once

/**
* Phosphor include/phosphor/texture.hpp
* Texture class
*/

#include <string>

#include <GL/glew.h>

class Texture {
protected:
    unsigned int id;
    int width, height, channels;
    std::string path;

public:
    Texture(unsigned char* data, int width, int height, int channels, const std::string& path):
        width(width), height(height), channels(channels), path(path) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        int texture_format = GL_RGB;
        if (channels == 4) {
            texture_format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, texture_format, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }
    ~Texture();

    void bind() const;
    void unbind();

    [[nodiscard]] inline int get_width() const { return width; }
    [[nodiscard]] inline int get_height() const { return height; }
    [[nodiscard]] inline int get_channels() const { return channels; }
    [[nodiscard]] inline std::string get_path() const { return path; }
};