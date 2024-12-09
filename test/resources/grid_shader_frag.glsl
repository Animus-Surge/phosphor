#version 430 core

// Phosphor Test - grid_shader_frag.glsl
// Grid fragment shader

in vec3 fragColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(fragColor, 1.0);
}

//EOF
