#version 430 core

layout(std140, binding = 2) uniform LightUniform {
    vec3 direction;
    vec3 color;
    float intensity;
    float falloff;
};

in vec3 fragColor;
in vec3 normal;
in vec3 viewDir;

out vec4 FragColor;

void main()
{
    FragColor = vec4(fragColor, 1.0);
}
