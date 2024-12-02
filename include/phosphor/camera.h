#pragma once

/**
 * Phosphor phosphor/camera.h
 * Camera system and declarations
 */

#include <glm/glm.hpp>

/**
 * Camera class
 */
class Camera {
protected:
    //Uniform buffer
    unsigned int cam_ubo;

    //Camera matrices
    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;
    glm::mat4 pv_matrix;

    //Camera properties
    float fov = 45.0f;
    float aspect_ratio;
    float near_clip = 0.1f;
    float far_clip = 100.0f;
    
    void generate_view_matrix();
    void generate_projection_matrix();
    void generate_pv_matrix();
    void regen_buffers();

    float last_anglex;
    float last_angley;

public:
    //Rotation
    float angle_x = 0.0f;
    float angle_y = 0.0f;
    
    //Camera vectors
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 direction = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 right;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    Camera(int aspect_width, int aspect_height);
    ~Camera();

    void set_aspect_ratio(int aspect_width, int aspect_height);
    void set_position(glm::vec3 position);
    void set_position(float x, float y, float z);

    //Better transform functions
    void translate(glm::vec3 translation);
    void translate(float x, float y, float z);

    void rotate(float x, float y);

    //no scale function, camera doesn't need it

    glm::mat4 get_view_matrix() { return view_matrix; }
    glm::mat4 get_projection_matrix() { return projection_matrix; }
    glm::mat4 get_pv_matrix() { return pv_matrix; }
}; // class Camera
