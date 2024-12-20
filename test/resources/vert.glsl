#version 450 core

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

//TODO: vertex lighting, materials

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 normal;
out vec3 viewDir;
out vec3 fragPos;

void main()
{
    gl_Position = pv_matrix * transform * vec4(aPos, 1.0);
    normal = mat3(transpose(inverse(transform))) * aNormal;
    viewDir = cam_position - vec3(transform * vec4(aPos, 1.0));
    fragPos = vec3(transform * vec4(aPos, 1.0));
}
