#version 450

layout(binding = 0) uniform TransformData {
    mat4 transform;
};
layout(binding = 1) uniform CameraData {
    mat4 view_matrix;
    mat4 projection_matrix;
    mat4 view_projection_matrix;
    vec3 cam_position;
    vec3 cam_direction;
    vec3 cam_right;
    vec3 cam_up;
};
layout(std140, binding = 2) uniform MaterialData {
    vec3 albedo;
    vec3 diffuse;
    vec3 specular;

    float metallic;
    float roughness;

    float has_texture;

    float mat_padding;
};
layout(std140, binding = 3) uniform LightData {
    vec3 light_position;
    vec3 light_color;
    float light_intensity;
    float light_radius;

    float light_padding[2];
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out vec4 frag_color;

vec3 blinnPhong(in vec3 vert_norm, in vec3 light_direction, in vec3 view_direction, in vec3 light_irradiance, in vec3 diffuse, in vec3 specular, in float roughness) {
    vec3 half_vec = normalize(light_direction + view_direction);
    vec3 spec = pow(max(dot(vert_norm, half_vec), 0.0), roughness) * specular;
    vec3 retc = diffuse * max(dot(vert_norm, light_direction), 0.0) + spec;
    retc *= light_irradiance;
    return retc;
}

vec3 calc_falloff(in vec3 intensity, in float falloff, in vec3 light_pos, in vec3 vert_pos) {
    float dist = distance(light_pos, vert_pos);
    return intensity / (falloff * dist * dist);
}

void main()
{
    gl_Position = view_projection_matrix * (transform * vec4(position, 1.0));

    vec4 tfm_norm = transform * vec4(normal, 0.0);

    vec3 pos = normalize(position);
    vec3 norm = normalize(tfm_norm.xyz);
    vec3 view_dir = normalize(cam_position - pos);
    vec3 light_dir = normalize(light_position - pos);

    vec3 ambient = albedo * vec3(0.1);

    vec3 light_irradiance = calc_falloff(light_color * light_intensity, light_radius, light_position, pos);

    vec3 color = blinnPhong(norm, light_dir, view_dir, light_irradiance, albedo, specular, roughness);
    frag_color = vec4(color + ambient, 1.0);

}