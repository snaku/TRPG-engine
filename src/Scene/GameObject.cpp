#include <Scene/IComponent.hpp>
#include <Scene/TransformComponents.hpp>
#include <Scene/GameObject.hpp>

namespace engine
{

GameObject::GameObject(std::string name, float xPos, float yPos, float zPos) 
    : name_(std::move(name)), pos_({xPos, yPos, zPos})
{
    data_.model = glm::mat4(1.0f);
    data_.position = pos_;
}

void GameObject::update(float deltaTime)
{
    for (const auto& component : components_)
    {
        if (component)
        {
            component->update(deltaTime);
        }
    }

    auto transform = getComponent<TransformComponent>();

    if (transform)
    {
        data_.model = transform->modelMatrix;
        data_.position = transform->position;
    }
}

}