#version 460 core
layout(std140, binding = 1) uniform LightUniform
{
    uniform vec3 light;
};

in vec3 color;
in vec3 norm;

layout (location=0) out vec4 out_FragColor;

void main()
{
    vec3 n = normalize(norm);
    vec3 l = normalize(light);

    float diffuse_intensity = clamp(dot(n, l) + 0.1, 0, 1);

    out_FragColor = vec4(color, 0.0) * diffuse_intensity;
}