#version 430 core

layout(binding = 0) uniform Transform {
    mat4 transform;
};
layout(binding = 1) uniform Camera {
    mat4 viewproj;
};

layout(location = 0) in vec3 vert_pos;
layout(location = 1) in vec3 vert_color;

out vec3 frag_color;

void main()
{
    gl_Position = viewproj * (transform * vec4(vert_pos, 1.0));
    frag_color = vert_color;
}
