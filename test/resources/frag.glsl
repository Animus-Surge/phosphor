#version 430 core

layout(std140, binding = 1) uniform MaterialUniform {
    vec4 albedo;
    vec4 emissive;
    float roughness;
    float specular;
};

layout(std140, binding = 3) uniform LightUniform {
    vec3 direction;
    vec3 color;
    float intensity;
    float falloff;
};

in vec3 normal;
in vec3 viewDir;

out vec4 FragColor;

void main()
{
    FragColor = vec4(albedo);
}
