#version 450

layout(binding = 2) uniform MaterialInfo {
    vec3 albedo;
    float metallic;
    float roughness;
};

layout(location = 0) out vec4 fragColor;

in vec3 frag_Normal;
in vec2 frag_UV;

void main() {
    fragColor = vec4(1.0);
}