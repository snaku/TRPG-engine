#pragma once

#include <glm/glm.hpp>

struct IComponent
{
    virtual void update(float deltaTime) = 0;
    virtual ~IComponent() = default;
};