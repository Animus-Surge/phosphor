#version 430 core

layout(binding = 0) uniform Transform {
    mat4 transform;
};
layout(binding = 1) uniform Camera {
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
