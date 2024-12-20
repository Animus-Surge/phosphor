#version 450 core

layout(std140, binding = 1) uniform MaterialUniform {
    vec4 albedo;
    vec4 emissive;
    float roughness;
    float specular;
};

layout(std140, binding = 3) uniform LightUniform {
    int type;
    vec3 light_color;
    float light_intensity;

    vec3 light_position;
    float light_radius;
    float light_constant_attenuation;
    float light_linear_attenuation;
    float light_quadratic_attenuation;

    vec3 light_direction;

    float light_cutoff;
    float light_outer_cutoff;
};

in vec3 normal;
in vec3 viewDir;
in vec3 fragPos;

out vec4 FragColor;

void main()
{
    if(type == 0) {
        vec3 n = normalize(normal);

        vec3 direction = normalize(light_position - fragPos);
        float distance = length(light_position - fragPos);

        float attenuation = 1.0 / (light_constant_attenuation +
            light_linear_attenuation * distance +
            light_quadratic_attenuation * (distance * distance));

        float diff = max(dot(n, direction), 0.0);
        vec3 diffuse = light_color * diff;

        vec3 viewNormal = normalize(viewDir);
        vec3 reflectDir = reflect(-direction, n);
        float spec = pow(max(dot(viewNormal, reflectDir), 0.0), specular);
        vec3 specular = light_color * spec;

        vec3 ambient = vec3(0.1) * albedo.rgb;

        vec3 result = (ambient + diffuse + specular);

        vec3 debug_normal = n * 0.5 + 0.5;

        FragColor = vec4(result, albedo.a);
    }
    else {
        FragColor = vec4(albedo);
    }
}
