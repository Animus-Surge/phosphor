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
     * @param vertex_path Path to the vertex shader
     * @param fragment_path Path to the fragment shader
     */
    Shader(const char* vertex_path, const char* fragment_path);

    /**
     * Destructor
     */
    ~Shader();

    /**
     * Use the shader
     */
    void use();

    /**
     * Set a boolean uniform
     * @param name Name of the uniform
     * @param value Value to set
     */
    void set_bool(const std::string &name, bool value) const;

    /**
     * Set an integer uniform
     * @param name Name of the uniform
     * @param value Value to set
     */
    void set_int(const std::string &name, int value) const;

    /**
     * Set a float uniform
     * @param name Name of the uniform
     * @param value Value to set
     */
    void set_float(const std::string &name, float value) const;
}; // class Shader
