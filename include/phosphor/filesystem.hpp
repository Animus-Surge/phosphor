#pragma once

/**
 * Phosphor include/phosphor/filesystem.hpp
 * Filesystem utilities for Phosphor.
 */

#include <cstring>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <spdlog/spdlog.h>

#include "phosphor/texture.hpp"
#include "phosphor/phosphor.hpp"

/**
* Reads a binary file into a vector of chars.
*
* @param filename The name of the file to read.
* @return A vector of chars containing the file data.
*/
inline PHOSPHOR_EXPORT std::vector<char> readBinFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        std::string error = strerror(errno);
        spdlog::error("Failed to open file '{}': {}", filename, error);

        return {};
    }
    spdlog::info("Reading file {}", filename);

    const size_t fileSize = file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

/**
* Reads a text file into a string.
*/
inline PHOSPHOR_EXPORT std::string readTextFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::string error = strerror(errno);
        spdlog::error("Failed to open file '{}': {}", filename, error);

        return "";
    }

    spdlog::info("Reading file {}", filename);

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();

    return content;
}

inline Texture* loadTexture(const std::string& filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);

    if (data == nullptr) {
        std::string error = stbi_failure_reason();
        spdlog::error("Failed to load texture '{}': {}", filename, error);
        return nullptr;
    }

    return new Texture(data, width, height, channels);
}