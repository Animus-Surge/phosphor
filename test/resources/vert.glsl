#version 430 core

layout(binding = 0) uniform Transform {
    mat4 transform;
};

layout(binding = 1) uniform Camera {
    mat4 viewproj;
};

layout(location = 0) in vec3 aPos;

void main()
{
    gl_Position = viewproj * (transform * vec4(aPos, 1.0));
}
