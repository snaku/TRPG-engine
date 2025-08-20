#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Scene/IComponent.hpp>

#include <iostream>

struct TransformComponent : IComponent
{
    void update(float deltaTime) override;

    // operator overloading
    bool operator==(const TransformComponent& other) const
    {
        return position == other.position &&
               rotation == other.rotation && 
               scale == other.scale;
    }
    bool operator!=(const TransformComponent& other) const
    {
        return !(*this == other);
    }
    TransformComponent& operator=(const TransformComponent& other)
    {
        // so we can't do something like transformComp = transformComp;
        if (this != &other)
        {
            rotation = other.rotation;
            position = other.position;
            scale = other.scale;
            modelMatrix = other.modelMatrix;
        }
            
        return *this;
    }

    glm::vec3 rotation{0.0f};
    glm::vec3 position{0.0f};
    glm::vec3 scale{1.0f};

    glm::mat4 modelMatrix{1.0f};
};