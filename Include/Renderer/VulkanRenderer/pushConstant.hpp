#pragma once

#include <glm/glm.hpp>

struct Push
{
    glm::mat4 model;
    glm::vec3 lightDir;
    float pad;
};