#version 450

layout(binding = 0) uniform TransformData {
    mat4 transform;
};
layout(binding = 1) uniform CameraData {
    mat4 view_projection_matrix;
};

layout(location = 0) in vec3 position;
//layout(location = 1) in vec3 normal;

void main()
{
    gl_Position = view_projection_matrix * transform * vec4(position, 1.0);
}