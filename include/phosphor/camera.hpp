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
    glm::vec3 direction;
    glm::vec3 right;
    glm::vec3 up;
};

class Camera {
    float last_angle_x, last_angle_y;
protected:
    glm::mat4 view_matrix, projection_matrix, view_projection_matrix;
    glm::vec3 position, direction, right;
    float fov = 45.0f;
    float aspect_ratio;
    float near = 0.1f;
    float far = 100.0f;
    float angle_x = 0.0f;
    float angle_y = 0.0f;

    unsigned int camera_ubo;

    void gen_matrices();
    void gen_ubo();
    void update_ubo();

public:
    Camera(int vp_width, int vp_height);
    ~Camera() {
        glDeleteBuffers(1, &camera_ubo);
    }

    void translate(glm::vec3 delta);
    void rotate_x(float delta);
    void rotate_y(float delta);

    glm::mat4 get_view_matrix() const {
        return view_matrix;
    }
    glm::mat4 get_projection_matrix() const {
        return projection_matrix;
    }
    glm::mat4 get_view_projection_matrix() const {
        return view_projection_matrix;
    }

    void update(float deltaT);
};