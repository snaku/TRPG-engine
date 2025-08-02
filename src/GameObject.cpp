#include "Include/IComponent.hpp"
#include "Include/TransformComponents.hpp"
#include "Include/GameObject.hpp"

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
    TransformComponent* transform = nullptr;

    for (const auto& component : components_)
    {
        if (component)
        {
            component->update(deltaTime);

            if (!transform)
            {
                transform = dynamic_cast<TransformComponent*>(component.get());
            }
        }
    }

    if (transform)
    {
        data_.model = transform->modelMatrix;
        data_.position = transform->position;
    }

    /*pos_ = glm::vec3(model_[3]);
    
    data_.model = model_;
    data_.position = pos_;*/
}

}