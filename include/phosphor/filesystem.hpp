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

#include <spdlog/spdlog.h>

/**
* Reads a binary file into a vector of chars.
*
* @param filename The name of the file to read.
* @return A vector of chars containing the file data.
*/
inline std::vector<char> readBinFile(const std::string& filename) {
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
inline std::string readTextFile(const std::string& filename) {
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