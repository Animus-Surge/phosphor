/**
* Phosphor src/camera.cpp
* Camera implementation
*/

#include "phosphor/camera.hpp"

#include <fmt/base.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const int vp_width, const int vp_height) {
    position = glm::vec3(0.0f, 0.0f, 4.0f);
    direction = glm::vec3(0.0f, 0.0f, -1.0f);
    right = glm::vec3(1.0f, 0.0f, 0.0f);

    fov = 45.0f;
    aspect_ratio = (float) vp_width / (float) vp_height;

    near = 0.1f;
    far = 100.0f;

    view_matrix = glm::lookAt(
        position,
        position + direction,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    projection_matrix = glm::perspective(
        glm::radians(fov),
        aspect_ratio,
        near,
        far
    );

    view_projection_matrix = projection_matrix * view_matrix;

    glGenBuffers(1, &camera_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(BindingCameraData), nullptr, GL_DYNAMIC_DRAW);

    glBindBufferBase(GL_UNIFORM_BUFFER, 1, camera_ubo);
}


