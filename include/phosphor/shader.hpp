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
    Shader(std::string vertex, std::string fragment);
    ~Shader() {
        glDeleteProgram(this->program);
    }

    void use() const {
        glUseProgram(this->program);
    }
};

unsigned int compileShader(unsigned int type, const std::string& source);