#pragma once

/**
* Phosphor include/phosphor/camera.hpp
* Camera class
*/

#include <GL/glew.h>
#include <glm/glm.hpp>

struct BindingCameraData {
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 view_projection;
    glm::vec3 position;
};

class Camera {
protected:
    glm::mat4 view_matrix, projection_matrix, view_projection_matrix;
    glm::vec3 position, direction, right;
    float fov;
    float aspect_ratio;
    float near;
    float far;

    unsigned int camera_ubo;

public:
    Camera(int vp_width, int vp_height);
    ~Camera() {
        glDeleteBuffers(1, &camera_ubo);
    }

    void set_position(glm::vec3 position);
    void set_rotation(glm::vec3 rotation);
    void set_fov(float fov);

    glm::mat4 get_view_matrix();
    glm::mat4 get_projection_matrix();
    glm::mat4 get_view_projection_matrix();

    void update();
};