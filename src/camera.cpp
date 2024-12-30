/**
* Phosphor src/camera.cpp
* Camera implementation
*/

#include "phosphor/camera.hpp"

#include <fmt/base.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Camera::gen_matrices() {
    view_matrix = glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f));
    projection_matrix = glm::perspective(glm::radians(fov), aspect_ratio, near, far);
    view_projection_matrix = projection_matrix * view_matrix;
}

void Camera::gen_ubo() {
    const BindingCameraData data = {
        view_matrix,
        projection_matrix,
        view_projection_matrix,
        position,
        direction,
        right,
        glm::vec3(0.0f, 1.0f, 0.0f)
    };

    glGenBuffers(1, &camera_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(BindingCameraData), &data, GL_DYNAMIC_DRAW);
}

void Camera::update_ubo() {
    const BindingCameraData data = {
        view_matrix,
        projection_matrix,
        view_projection_matrix,
        position,
        direction,
        right,
        glm::vec3(0.0f, 1.0f, 0.0f)
    };

    glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(BindingCameraData), &data);
}

Camera::Camera(const int vp_width, const int vp_height) {
    position = glm::vec3(0.0f, 0.0f, 4.0f);
    direction = glm::vec3(0.0f, 0.0f, -1.0f);
    right = glm::vec3(1.0f, 0.0f, 0.0f);

    fov = 45.0f;
    aspect_ratio = (float) vp_width / (float) vp_height;

    near = 0.1f;
    far = 100.0f;

    gen_matrices();
    gen_ubo();

    glBindBufferBase(GL_UNIFORM_BUFFER, 1, camera_ubo);

    last_angle_x = angle_x;
    last_angle_y = angle_y;
}

void Camera::translate(const glm::vec3 delta) {
    position += delta;
    gen_matrices();
    update_ubo();
}

void Camera::rotate_x(const float delta) {
    angle_x += delta;
}

void Camera::rotate_y(const float delta) {
    angle_y += delta;
}

void Camera::update(float deltaT) {
    if (last_angle_x != angle_x || last_angle_y != angle_y) {
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle_x, glm::vec3(1.0f, 0.0f, 0.0f));
        rotation = glm::rotate(rotation, angle_y, glm::vec3(0.0f, 1.0f, 0.0f));

        direction = glm::vec3(rotation * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
        right = glm::vec3(rotation * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        gen_matrices();
        update_ubo();
    }
}

