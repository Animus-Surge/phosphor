#pragma once

/**
 * Phosphor phosphor/shader.h
 * Shader system and declarations
 */

#include <string>

/**
 * Shader class
 */
class Shader {
private:
    /**
     * Shader program ID, set to -1 by default (invalid)
     */
    unsigned int id = -1;
public:
    /**
     * Constructor
     * @param vertex Path to the vertex shader, or source code if from_file is false
     * @param fragment Path to the fragment shader, or source code if from_file is false
     * @param from_file Whether the paths are files or source code
     */
    Shader(std::string vertex, std::string fragment, bool from_file = true);

    /**
     * Destructor
     */
    ~Shader();

    /**
     * Use the shader
     */
    void use();

    /**
     * Drop the shader
     */
    void drop();

}; // class Shader
