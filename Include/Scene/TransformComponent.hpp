#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Scene/IComponent.hpp>

#include <iostream>

struct TransformComponent : IComponent
{
    TransformComponent();
    TransformComponent(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& sca);
    TransformComponent(const TransformComponent& other);

    void update([[maybe_unused]] float deltaTime) override;

    const glm::mat4& getModelMatrix() const { return modelMatrix; }

    // operator overloading
    TransformComponent& operator=(const TransformComponent& other);
    bool operator==(const TransformComponent& other) const;
    bool operator!=(const TransformComponent& other) const;

    // they are public because it's easier to use them like that
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

private:
    glm::mat4 modelMatrix{1.0f};
};