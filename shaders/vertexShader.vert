#version 450

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPos;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 normal;

layout(set = 0, binding = 0) uniform Ubo
{
    // mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(push_constant) uniform Push
{
    mat4 model;
    vec3 lightDir;

    // a vec3 is 12 bytes but is aligned to 16 bytes so we add this float
    // to match the CPU memory layout 
    float pad; 
} push;

void main() 
{
    vec4 pos = vec4(inPosition, 1.0);
    gl_Position = ubo.proj * ubo.view * push.model * pos;

    vec3 normalWorldSpace = normalize(mat3(push.model) * normal);
    vec3 lightDirWorld = normalize(push.lightDir);
    float lightIntensity = max(dot(normalWorldSpace, -lightDirWorld), 0.0);
    const float ambient = 0.2;

    fragColor = (ambient + lightIntensity) * inColor;
    fragPos = (push.model * vec4(inPosition, 1.0)).xyz;
}