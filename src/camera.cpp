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
    this->cameraData.view_matrix = glm::lookAt(
            this->cameraData.cam_position,
            this->cameraData.cam_position + this->cameraData.cam_direction,
            glm::vec3(0.0f, 1.0f, 0.0f)
            );
}
void Camera::generate_projection_matrix() {
    this->cameraData.projection_matrix = glm::perspective(
            this->cameraData.cam_fov,
            this->cameraData.cam_aspect_ratio,
            this->cameraData.cam_near_clip,
            this->cameraData.cam_far_clip
            );
}
void Camera::generate_pv_matrix() {
    this->cameraData.pv_matrix = this->cameraData.projection_matrix * this->cameraData.view_matrix;
}

void Camera::regen_buffers() {
    //Rebind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, this->cam_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraUniform), &this->cameraData, GL_STATIC_DRAW);

    //Rebind UBO
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->cam_ubo);
}

Camera::Camera(int aspect_width, int aspect_height) { //TODO: add arguments
    this->cameraData.cam_aspect_ratio = (float)aspect_width / (float)aspect_height; 

    //Normalize vectors
    this->cameraData.cam_direction = glm::normalize(this->cameraData.cam_direction);

    //Compute right vector
    this->cameraData.cam_right = glm::normalize(glm::cross(this->cameraData.cam_direction, glm::vec3(0.0f, 1.0f, 0.0f)));

    //Generate initial angle values
    this->angle_x = atan2(this->cameraData.cam_direction.z, this->cameraData.cam_direction.x);
    this->angle_y = asin(this->cameraData.cam_direction.y);

    //Create matrices
    this->generate_view_matrix();
    this->generate_projection_matrix();
    this->generate_pv_matrix();

    //Create UBO
    glGenBuffers(1, &this->cam_ubo);
    this->regen_buffers();
}

void Camera::set_aspect_ratio(int aspect_width, int aspect_height) {
    this->cameraData.cam_aspect_ratio = (float)aspect_width / (float)aspect_height;
    this->generate_projection_matrix();
    this->generate_pv_matrix();
    this->regen_buffers();
}

void Camera::set_position(glm::vec3 position) {
    this->cameraData.cam_position = position;
    this->generate_view_matrix();
    this->generate_pv_matrix();
    this->regen_buffers();
}

void Camera::set_position(float x, float y, float z) {
    this->cameraData.cam_position = glm::vec3(x, y, z); 
    this->generate_view_matrix();
    this->generate_pv_matrix();
    this->regen_buffers();
}

Camera::~Camera() {
    //Cleanup
    glDeleteBuffers(1, &this->cam_ubo);
}

void Camera::translate(glm::vec3 position) {
    this->cameraData.cam_position += position;
    this->generate_view_matrix();
    this->generate_pv_matrix();
    this->regen_buffers();
}

void Camera::translate(float x, float y, float z) {
    this->cameraData.cam_position += glm::vec3(x, y, z);
    this->generate_view_matrix();
    this->generate_pv_matrix();
    this->regen_buffers();
}

//x and y are already scaled and clamped
void Camera::rotate(float x, float y) {
    this->angle_x -= x * 0.005f;
    this->angle_y -= y * 0.005f;

    //Clamp y angle
    if(this->angle_y > M_PI / 2) {
        this->angle_y = M_PI / 2;
    } else if(this->angle_y < -M_PI / 2) {
        this->angle_y = -M_PI / 2;
    }

    //Wrap x angle
    if(this->angle_x > 2 * M_PI) {
        this->angle_x -= 2 * M_PI;
    } else if(this->angle_x < 0) {
        this->angle_x += 2 * M_PI;
    }
    
    //Update vectors
    this->cameraData.cam_direction = glm::normalize(glm::vec3(
                cos(this->angle_y) * sin(this->angle_x),
                sin(this->angle_y),
                cos(this->angle_y) * cos(this->angle_x)
                ));

    this->cameraData.cam_right = glm::normalize(glm::cross(this->cameraData.cam_direction, glm::vec3(0.0f, 1.0f, 0.0f)));

    //Regenerate
    this->generate_view_matrix();
    this->generate_pv_matrix();
    this->regen_buffers();
}

