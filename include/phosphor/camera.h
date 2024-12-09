#pragma once

/**
 * Phosphor phosphor/camera.h
 * Camera system and declarations
 */

#include <glm/glm.hpp>

struct CameraUniform {
    glm::mat4 view_matrix = glm::mat4(1.0f);
    glm::mat4 projection_matrix = glm::mat4(1.0f);
    glm::mat4 pv_matrix = glm::mat4(1.0f);
    glm::vec3 cam_position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cam_direction = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 cam_right;

    float cam_fov = 45.0f;
    float cam_aspect_ratio = 1366.0f/768.0f;
    float cam_near_clip = 0.1f;
    float cam_far_clip = 100.0f;
};

/**
 * Camera class
 */
class Camera {
protected:
    //Uniform buffer
    unsigned int cam_ubo;

    void generate_view_matrix();
    void generate_projection_matrix();
    void generate_pv_matrix();
    void regen_buffers();

public:
    CameraUniform cameraData;

    //Rotation
    float angle_x = 0.0f;
    float angle_y = 0.0f;
    
    //Camera vectors
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    Camera(int aspect_width, int aspect_height);
    ~Camera();

    void set_aspect_ratio(int aspect_width, int aspect_height);
    void set_position(glm::vec3 position);
    void set_position(float x, float y, float z);

    void set_direction(glm::vec3 direction) {
        this->cameraData.cam_direction = direction;

        //Regenerate angles
        this->angle_x = atan2(this->cameraData.cam_direction.z, this->cameraData.cam_direction.x);
        this->angle_y = asin(this->cameraData.cam_direction.y);

        this->generate_view_matrix();
        this->generate_pv_matrix();
        this->regen_buffers();
    }
    void set_direction(float x, float y, float z) {
        this->cameraData.cam_direction = glm::vec3(x, y, z);

        //Regenerate angles
        
        //-z axis: 0 rad on x
        //+z axis: pi rad on x

        this->angle_x = atan2(this->cameraData.cam_direction.z, this->cameraData.cam_direction.x);
        this->angle_y = asin(this->cameraData.cam_direction.y);

        this->generate_view_matrix();
        this->generate_pv_matrix();
        this->regen_buffers();
    }

    //Better transform functions
    void translate(glm::vec3 translation);
    void translate(float x, float y, float z);
    void rotate(float x, float y);

    glm::vec3 get_position() { return cameraData.cam_position; }
    glm::vec3 get_direction() { return cameraData.cam_direction; }
    glm::vec3 get_right() { return cameraData.cam_right; }

    glm::mat4 get_view_matrix() { return cameraData.view_matrix; }
    glm::mat4 get_projection_matrix() { return cameraData.projection_matrix; }
    glm::mat4 get_pv_matrix() { return cameraData.pv_matrix; }
}; // class Camera
