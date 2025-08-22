#include <Scene/Components/TransformComponent.hpp>
#include <Scene/Components/PhysicsComponent.hpp>

PhysicsComponent::PhysicsComponent(TransformComponent* transform) : transform_(transform)
{
}

void PhysicsComponent::update(float deltaTime)
{
    velocity += (acceleration + gravity) * deltaTime;

    transform_->position += velocity * deltaTime;

    if (transform_->position.y <= 2.0f)
    {
        transform_->position.y = 2.0f;
        velocity.y *= -0.7f;
    }

    acceleration = glm::vec3(0.0f);
}