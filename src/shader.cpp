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

#include "phosphor/shader.h"

Shader::Shader(const char* vertex_path, const char* fragment_path) {
    //Code and file reading
    std::string vertex_code, fragment_code;
    std::ifstream vertex_file, fragment_file;

    int success;
    char info_log[512];

    //Throw exceptions on file read failure
    vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    //Read files
    try {
        vertex_file.open(vertex_path);
        fragment_file.open(fragment_path);

        std::stringstream vertex_stream, fragment_stream;

        vertex_stream << vertex_file.rdbuf();
        fragment_stream << fragment_file.rdbuf();

        vertex_file.close();
        fragment_file.close();

        vertex_code = vertex_stream.str();
        fragment_code = fragment_stream.str();
    } catch(std::ifstream::failure e) {
        printf("Error reading shader file: %s\n", e.what());
    }

    //Compile shaders
    const char* vertex_shader = vertex_code.c_str();
    const char* fragment_shader = fragment_code.c_str();

    unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &vertex_shader, NULL);
    glCompileShader(vertex_shader_id);

    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex_shader_id, 512, NULL, info_log);
        printf("Vertex shader compilation failed: %s\n", info_log);
        return;
    }

    unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &fragment_shader, NULL);
    glCompileShader(fragment_shader_id);

    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment_shader_id, 512, NULL, info_log);
        printf("Fragment shader compilation failed: %s\n", info_log);
        return;
    }

    this->id = glCreateProgram();
    glAttachShader(this->id, vertex_shader_id);
    glAttachShader(this->id, fragment_shader_id);
    glLinkProgram(this->id);

    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(this->id, 512, NULL, info_log);
        printf("Shader program linking failed: %s\n", info_log);
        this->id = -1;
        return;
    }
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
