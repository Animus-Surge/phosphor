#version 450

layout(location = 0) out vec4 fragColor;

//Vertex shader inputs
in vec4 frag_color;

void main() {
    fragColor = frag_color;
}