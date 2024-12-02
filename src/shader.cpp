/**
 * Phosphor shader.cpp
 * Shader system implementation
 */

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

#include <GL/glew.h>
#include "spdlog/spdlog.h"
#include "spdlog/fmt/std.h"

#include "phosphor/shader.h"

Shader::Shader(const char* vertex_path, const char* fragment_path) {
    //Code and file reading
    std::string vertex_code, fragment_code;
    std::ifstream vertex_file, fragment_file;

    int success;
    char info_log[512];

    //Print working directory
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    spdlog::debug("Current working directory: {}", cwd);

    //Combine cwd with shader paths
    std::string vertex_path_str = std::string(vertex_path);
    std::string fragment_path_str = std::string(fragment_path);

    std::filesystem::path cwd_path(cwd);
    std::filesystem::path vertex_path_fs(vertex_path_str);
    std::filesystem::path fragment_path_fs(fragment_path_str);

    std::filesystem::path vertex_full_path = cwd_path / vertex_path_fs;
    std::filesystem::path fragment_full_path = cwd_path / fragment_path_fs;

    //Throw exceptions on file read failure
    vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    //Read files
    try {
        vertex_file.open(vertex_full_path);
        fragment_file.open(fragment_full_path);

        std::stringstream vertex_stream, fragment_stream;

        vertex_stream << vertex_file.rdbuf();
        fragment_stream << fragment_file.rdbuf();

        vertex_file.close();
        fragment_file.close();

        vertex_code = vertex_stream.str();
        fragment_code = fragment_stream.str();
    } catch(std::ifstream::failure e) {
        spdlog::error("Shader file read failed: {}", e.what());

        spdlog::error("Vertex shader path: {}", vertex_full_path);
        spdlog::error("Fragment shader path: {}", fragment_full_path);
        return;
    }

    spdlog::info("Loaded vertex shader from {}", fmt::ptr(vertex_path));
    spdlog::info("Loaded fragment shader from {}", fmt::ptr(fragment_path));

    //Compile shaders
    const char* vertex_shader = vertex_code.c_str();
    const char* fragment_shader = fragment_code.c_str();

    unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &vertex_shader, NULL);
    glCompileShader(vertex_shader_id);

    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex_shader_id, 512, NULL, info_log);
        spdlog::error("Vertex shader compilation failed: {}", fmt::ptr(info_log));
        return;
    }

    unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &fragment_shader, NULL);
    glCompileShader(fragment_shader_id);

    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment_shader_id, 512, NULL, info_log);
        spdlog::error("Fragment shader compilation failed: {}", fmt::ptr(info_log));
        return;
    }

    this->id = glCreateProgram();
    glAttachShader(this->id, vertex_shader_id);
    glAttachShader(this->id, fragment_shader_id);
    glLinkProgram(this->id);

    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(this->id, 512, NULL, info_log);
        spdlog::error("Shader program linking failed: {}", fmt::ptr(info_log));
        this->id = -1;
        return;
    }

    spdlog::info("Shader program created with ID {}", this->id);
}

Shader::~Shader() {
    glDeleteProgram(this->id);
}

void Shader::use() {
    glUseProgram(this->id);
}

void Shader::drop() {
    glUseProgram(0);
}

//TODO: implement uniforms

void Shader::set_bool(const std::string &name, bool value) const {
}

void Shader::set_int(const std::string &name, int value) const {
}

void Shader::set_float(const std::string &name, float value) const {
}
