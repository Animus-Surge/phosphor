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

#include "phosphor/shader.hpp"

Shader::Shader(std::string vertex, std::string fragment, bool from_file)
{
    std::string vertex_code, fragment_code;

    int success;
    char info_log[1024];

    // Code and file reading
    if (from_file)
    {
        std::ifstream vertex_file, fragment_file;

        // Throw exceptions on file read failure
        vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        // Read files
        try
        {
            vertex_file.open(vertex);
            fragment_file.open(fragment);

            std::stringstream vertex_stream, fragment_stream;

            vertex_stream << vertex_file.rdbuf();
            fragment_stream << fragment_file.rdbuf();

            vertex_file.close();
            fragment_file.close();

            vertex_code = vertex_stream.str();
            fragment_code = fragment_stream.str();
        }
        catch (std::ifstream::failure e)
        {
            spdlog::error("Shader file read failed: {}", e.what());

            spdlog::error("Vertex shader path: {}", vertex);
            spdlog::error("Fragment shader path: {}", fragment);
            return;
        }

        spdlog::info("Loaded vertex shader from {}", vertex);
        spdlog::info("Loaded fragment shader from {}", fragment);
    } else {
        vertex_code = vertex;
        fragment_code = fragment;
    }

    // Compile shaders
    const char *vertex_shader = vertex_code.c_str();
    const char *fragment_shader = fragment_code.c_str();

    unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &vertex_shader, NULL);
    glCompileShader(vertex_shader_id);

    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader_id, 512, NULL, info_log);
        std::string info_log_str(info_log);
        spdlog::error("Vertex shader compilation failed: {}", info_log_str);
        return;
    }

    unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &fragment_shader, NULL);
    glCompileShader(fragment_shader_id);

    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader_id, 512, NULL, info_log);
        std::string info_log_str(info_log);
        spdlog::error("Fragment shader compilation failed: {}", info_log_str);
        return;
    }

    this->id = glCreateProgram();
    glAttachShader(this->id, vertex_shader_id);
    glAttachShader(this->id, fragment_shader_id);
    glLinkProgram(this->id);

    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->id, 512, NULL, info_log);
        std::string info_log_str(info_log);
        spdlog::error("Shader program linking failed: {}", info_log_str);
        this->id = -1;
        return;
    }

    spdlog::info("Shader program created with ID {}", this->id);
}

Shader::~Shader()
{
    glDeleteProgram(this->id);
}

void Shader::use()
{
    glUseProgram(this->id);
}

void Shader::drop()
{
    glUseProgram(0);
}

