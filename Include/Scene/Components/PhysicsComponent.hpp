#pragma once

#include <glm/glm.hpp>

#include <Scene/Components/IComponent.hpp>

struct TransformComponent;

struct PhysicsComponent : IComponent
{
    explicit PhysicsComponent(TransformComponent* transform);

    void update(float deltaTime) override;

private:
    TransformComponent* transform_ = nullptr;

    glm::vec3 velocity{0.0f};
    glm::vec3 acceleration{0.0f};
    const glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);
};