#pragma once

/**
* Phosphor include/phosphor/shader.hpp
* Shader class
*/

#include <fstream>
#include <string>

#include <GL/glew.h>

class Shader {
protected:
    unsigned int program;
public:
    Shader(const std::string &vertex, const std::string &fragment, bool is_file = true);
    ~Shader() {
        glDeleteProgram(this->program);
    }

    void use() const {
        glUseProgram(this->program);
    }

    void drop() const {
        glUseProgram(0);
    }
};

unsigned int compileShader(unsigned int type, const std::string& source);