/**
* Phosphor src/shader.cpp
* Shader implementation
*/

#include "phosphor/filesystem.hpp"
#include "phosphor/shader.hpp"

#include <spdlog/spdlog.h>

Shader::Shader(const std::string &vertex, const std::string &fragment, const bool is_file) {
    std::string vertexSource;
    std::string fragmentSource;

    if (is_file) {
        spdlog::info("Loading shader from file: {} {}", vertex, fragment);
        vertexSource = readTextFile(vertex);
        fragmentSource = readTextFile(fragment);
    } else {
        vertexSource = vertex;
        fragmentSource = fragment;
    }

    const unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    const unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    this->program = glCreateProgram();
    glAttachShader(this->program, vertexShader);
    glAttachShader(this->program, fragmentShader);
    glLinkProgram(this->program);

    int success;
    glGetProgramiv(this->program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(this->program, 512, nullptr, infoLog);
        std::string error = infoLog;
        spdlog::error("Shader program linking failed: {}", error);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

unsigned int compileShader(const unsigned int type, const std::string& source) {
    const unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::string error = infoLog;
        spdlog::error("Shader compilation failed: {}", error);
    }

    return shader;
}