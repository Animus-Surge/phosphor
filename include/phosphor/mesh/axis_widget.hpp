#pragma once

/**
 * Phosphor phosphor/mesh/axis_widget.h
 * Axis widget mesh class
 */

#include "phosphor/mesh/mesh.hpp"
#include "phosphor/shader.hpp"

const std::string axis_vert = R"(
#version 430 core

layout(binding = 0) uniform Transform {
    mat4 transform;
};
layout(binding = 2) uniform Camera {
    mat4 view_matrix;
    mat4 projection_matrix;
    mat4 pv_matrix;
    vec3 cam_position;
    vec3 cam_direction;
    vec3 cam_right;

    float cam_fov;
    float cam_aspect_ratio;
    float cam_near_clip;
    float cam_far_clip;
};

layout(location = 0) in vec3 vert_pos;
layout(location = 1) in vec3 vert_color;

out vec3 frag_color;

void main()
{
    gl_Position = pv_matrix * (transform * vec4(vert_pos, 1.0));
    frag_color = vert_color;
}
)";

const std::string axis_frag = R"(
#version 430 core

in vec3 frag_color;

out vec4 FragColor;

void main() {
FragColor = vec4(frag_color, 1.0);
}
)";

class AxisWidget : public Mesh {
protected:
    Shader* axis_shader;
public:
    /**
     * Constructor
     */
    AxisWidget();

    /**
     * Destructor
     */
    ~AxisWidget();

    /**
     * Render the axis widget
     */
    void render() override;
    
    void regen_ubo() override {
    }

    void set_transform(glm::mat4 transform);
}; // class AxisWidget
