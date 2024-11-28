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
    unsigned int cam_ubo;

    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;
    glm::mat4 pv_matrix;

    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 right;

    float fov;
    float aspect_ratio;
    float near_clip;
    float far_clip;
    float angle_x;
    float angle_y;

public:
    Camera();
    ~Camera();

    glm::mat4 get_view_matrix() { return view_matrix; }
    glm::mat4 get_projection_matrix() { return projection_matrix; }
    glm::mat4 get_pv_matrix() { return pv_matrix; }

    void render();

    void set_position(glm::vec3 position); //TODO
}; // class Camera
