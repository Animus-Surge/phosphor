#pragma once

/**
 * Phosphor include/phosphor/filesystem.hpp
 * Filesystem utilities for Phosphor.
 */

#include <fstream>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>

/**
 * Reads a text file into a string.
 *
 * @param filename The name of the file to read.
 * @return The contents of the file as a char vector.
 */
std::vector<char> readBinFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file.");
    }
    spdlog::info("Reading file {}", filename);

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}