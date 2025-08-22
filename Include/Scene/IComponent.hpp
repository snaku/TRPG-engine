#pragma once

#include <glm/glm.hpp>

struct IComponent
{
    virtual ~IComponent() = default;
    virtual void update(float deltaTime) = 0;
};