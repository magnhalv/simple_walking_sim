#version 460 core
layout(std140, binding = 1) uniform LightData
{
    vec4 omni_pos_vec4;
    vec4 eye_pos_vec4;
};

uniform vec4 mat_diffuse;
uniform vec4 mat_spec;

in vec3 position;
in vec3 norm;

layout (location=0) out vec4 out_FragColor;

void main()
{
    vec3 omni_pos = omni_pos_vec4.xyz;
    float inv_omni_radius = omni_pos_vec4.w;

    vec3 eye_pos = eye_pos_vec4.xyz;
    vec3 n = normalize(norm);

    vec3 v = normalize(eye_pos - position);

    float specular_exponent = 100;
    vec3 mat_emis = vec3(0.0, 0.0, 0.0);
    vec3 omni_color = vec3(1.0, 1.0, 1.0);
    float global_ambient = 0.4;

    vec3 L = omni_pos - position;
    float dist = length(L);

    vec3 l = normalize(L);

    vec3 light_color = omni_color * max(1 - dist * inv_omni_radius, 0);

    float diff_factor = max(dot(n, l), 0);

    vec3 h = normalize(v + l);
    float spec_factor = max(pow(max(dot(n, h), 0.0), specular_exponent), 0.0);

    vec3 c_spec = (light_color * mat_spec.xyz) * spec_factor;
    vec3 c_diff = (light_color * mat_diffuse.xyz) * diff_factor;
    vec3 c_amb = global_ambient * mat_diffuse.xyz;
    vec3 c_emis = mat_emis;

    out_FragColor = vec4(c_spec + c_diff + c_amb + c_emis, 1.0);
}