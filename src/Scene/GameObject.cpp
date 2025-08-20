#include <Scene/IComponent.hpp>
#include <Scene/TransformComponent.hpp>
#include <Scene/GameObject.hpp>

namespace engine
{

GameObject::GameObject(const std::string& name) 
    : name_(name)
{
    data_.model = glm::mat4(1.0f);
    data_.position = glm::vec3(0.0f, 0.0f, 0.0f);
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

    if (!transform_)
    {
        transform_ = getComponent<TransformComponent>();
    }
    
    if (transform_)
    {
        data_.model = transform_->getModelMatrix();
        data_.position = transform_->position;
    }
}

}