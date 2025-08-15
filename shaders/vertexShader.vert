#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPos;

layout(set = 0, binding = 0) uniform Ubo
{
    // mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(push_constant) uniform Push
{
    mat4 model;
} push;

void main() 
{
    vec4 pos = vec4(inPosition, 1.0);
    gl_Position = ubo.proj * ubo.view * push.model * pos;
    fragColor = inColor;
    fragPos = inPosition;
}