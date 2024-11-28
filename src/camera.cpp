/**
 * Phosphor camera.cpp
 * Camera implementation
 */

#include <cmath>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "phosphor/camera.h"

Camera::Camera() { //TODO: add arguments
    //Init

    this->angle_x = (float)M_PI;
    this->angle_y = 0.0f;
    this->position = glm::vec3(0.0f, 0.0f, 12.0f); //Up 12 units
    this->direction = glm::vec3(0.0f, 0.0f, -1.0f); //Look down
    this->right = glm::vec3(1.0f, 0.0f, 0.0f); //Right (positive x)
    this->fov = 45.0f;
    this->aspect_ratio = 800.0f / 600.0f;
    this->near_clip = 0.1f;
    this->far_clip = 100.0f;

    //Create matrices
    this->view_matrix = glm::lookAt(
            this->position,
            this->position + this->direction,
            cross(this->direction, this->right)
            );

    this->projection_matrix = glm::perspective(
            this->fov,
            this->aspect_ratio,
            this->near_clip,
            this->far_clip
            );

    this->pv_matrix = this->projection_matrix * this->view_matrix;

    //Create UBO
    glGenBuffers(1, &this->cam_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, this->cam_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3, &this->pv_matrix, GL_STATIC_DRAW);

    //Bind UBO
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->cam_ubo);
}

Camera::~Camera() {
    //Cleanup
    glDeleteBuffers(1, &this->cam_ubo);
}

void render() {
}

