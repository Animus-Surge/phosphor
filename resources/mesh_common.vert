#version 450

layout(binding = 0) uniform TransformData {
    mat4 transform;
};
layout(binding = 1) uniform CameraData {
    mat4 view_matrix;
    mat4 projection_matrix;
    mat4 view_projection_matrix;
    vec3 cam_position;
    vec3 cam_direction;
    vec3 cam_right;
    vec3 cam_up;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out vec3 frag_Normal;
out vec2 frag_UV;

void main()
{
    gl_Position = view_projection_matrix * (transform * vec4(position, 1.0));

    frag_Normal = normal;
    frag_UV = uv;
}