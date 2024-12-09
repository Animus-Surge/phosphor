#version 430 core

// Phosphor Test - grid_shader_vert.glsl
// Grid vertex shader

layout(binding = 0) uniform Transform {
    mat4 transform;
};
layout(binding = 1) uniform CameraData {
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

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;

out vec3 fragColor;

void main()
{
    gl_Position = pv_matrix * transform * vec4(aPos, 1.0);
    fragColor = aColor;
}

//EOF
