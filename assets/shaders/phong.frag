#version 460 core
layout(std140, binding = 1) uniform LightUniform
{
    vec3 omni_pos;
    vec3 eye_pos;
};

in vec3 position;
in vec3 norm;

layout (location=0) out vec4 out_FragColor;

void main()
{
    vec3 n = normalize(norm);
    vec3 v = normalize(eye_pos - position);
    float specular_exponent = 1000;

    vec3 mat_diff = vec3(0.6, 0.3, 0.3);
    vec3 source_diff = vec3(1.0, 1.0, 1.0);
    vec3 source_spec = source_diff;
    vec3 mat_spec = vec3(0.9, 0.1, 0.1);


    vec3 l = normalize(omni_pos - position);
    float diff_intensity = max(dot(n, l), 0);

    vec3 r = 2*diff_intensity*n - l;
    vec3 h = normalize(v + l);
    float spec_factor = pow(max(dot(n, h), 0), specular_exponent);

    vec3 c_spec = (source_spec * mat_spec) * spec_factor;
    vec3 c_diff = (source_diff * mat_diff) * diff_intensity;
    out_FragColor = vec4(c_spec + c_diff, 0);
}