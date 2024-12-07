/**
 * Phosphor camera.cpp
 * Camera implementation
 */

#include <cmath>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

#include "phosphor/camera.h"

//TODO: input event handler, componentize

void Camera::generate_view_matrix() {
    this->view_matrix = glm::lookAt(
            this->position,
            this->position + this->direction,
            glm::vec3(0.0f, 1.0f, 0.0f)
            );
}
void Camera::generate_projection_matrix() {
    this->projection_matrix = glm::perspective(
            this->fov,
            this->aspect_ratio,
            this->near_clip,
            this->far_clip
            );
}
void Camera::generate_pv_matrix() {
    this->pv_matrix = this->projection_matrix * this->view_matrix;
}

void Camera::regen_buffers() {
    //Rebind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, this->cam_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3, &this->pv_matrix, GL_STATIC_DRAW);

    //Rebind UBO
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->cam_ubo);
}

Camera::Camera(int aspect_width, int aspect_height) { //TODO: add arguments
    this->aspect_ratio = (float)aspect_width / (float)aspect_height; 

    //Normalize vectors
    this->direction = glm::normalize(this->direction);

    //Compute right vector
    this->right = glm::normalize(glm::cross(this->direction, glm::vec3(0.0f, 1.0f, 0.0f)));

    //Create matrices
    this->generate_view_matrix();
    this->generate_projection_matrix();
    this->generate_pv_matrix();

    //Create UBO
    glGenBuffers(1, &this->cam_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, this->cam_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3, &this->pv_matrix, GL_STATIC_DRAW);

    //Bind UBO
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->cam_ubo);
}

void Camera::set_aspect_ratio(int aspect_width, int aspect_height) {
    this->aspect_ratio = (float)aspect_width / (float)aspect_height;
    this->generate_projection_matrix();
    this->generate_pv_matrix();
    this->regen_buffers();
}

void Camera::set_position(glm::vec3 position) {
    this->position = position;
    this->generate_view_matrix();
    this->generate_pv_matrix();
    this->regen_buffers();
}

void Camera::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z); 
    this->generate_view_matrix();
    this->generate_pv_matrix();
    this->regen_buffers();
}

Camera::~Camera() {
    //Cleanup
    glDeleteBuffers(1, &this->cam_ubo);
}

void Camera::translate(glm::vec3 position) {
    this->position += position;
    this->generate_view_matrix();
    this->generate_pv_matrix();
    this->regen_buffers();
}

void Camera::translate(float x, float y, float z) {
    this->position += glm::vec3(x, y, z);
    this->generate_view_matrix();
    this->generate_pv_matrix();
    this->regen_buffers();
}

//x and y are already scaled and clamped
void Camera::rotate(float x, float y) {
    this->angle_x -= x * 0.005f;
    this->angle_y -= y * 0.005f;

    //Clamp y angle
    this->angle_y = glm::clamp(this->angle_y, -1.0f, 1.0f);

    this->direction = glm::normalize(glm::vec3(
                cos(this->angle_y) * sin(this->angle_x),
                sin(this->angle_y),
                cos(this->angle_y) * cos(this->angle_x)
                ));

    this->right = glm::normalize(glm::cross(this->direction, glm::vec3(0.0f, 1.0f, 0.0f)));

    this->generate_view_matrix();
    this->generate_pv_matrix();
    this->regen_buffers();
}

