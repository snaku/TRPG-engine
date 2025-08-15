#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPos;

layout(location = 0) out vec4 outColor;

void main()
{
    //vec3 finalColor;
    vec3 center = vec3(0.3, -0.1, 0.0);
    float dist = length(fragPos - center);
    float radius = 0.3;
    
    /*if (dist < radius)
        finalColor = vec3(1.0); // white
    else 
        discard;*/

    float intensity = 1.0 - clamp(dist / 1.0, 0.0, 1.0);

    vec3 finalColor = fragColor * intensity;
    outColor = vec4(fragColor, 1.0);
}