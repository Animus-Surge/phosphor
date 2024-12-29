#version 450

layout(location = 0) out vec4 fragColor;

in vec3 fragPos;

void main() {
    vec3 finalColor = fragPos + 0.5 / 2.0;
    fragColor = vec4(finalColor, 1.0);
}