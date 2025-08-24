#include <Scene/Components/IComponent.hpp>
#include <Scene/Components/TransformComponent.hpp>
#include <Scene/GameObject.hpp>

namespace engine
{

GameObject::GameObject(const std::string& name) 
    : name_(name)
{
    data_.model = glm::mat4(1.0f);
    data_.position = glm::vec3(0.0f, 0.0f, 0.0f);
}

void GameObject::addParent(GameObject& parent)
{
    if (this != &parent)
    {
        if (parent_) 
            parent_->removeChild(*this);
        
        parent_ = &parent;
    }
}

void GameObject::removeParent(GameObject& parent)
{
    if (parent_ == &parent)
    {
        parent_ = nullptr;
    }
}

void GameObject::addChild(GameObject& child)
{
    if (this == &child) return;
    if (child.isAChildOf(*this)) return;

    children_.push_back(&child);
    child.addParent(*this);
}

void GameObject::removeChild(GameObject& child)
{
    if (!child.isAChildOf(*this)) return;

    auto it = std::remove_if(children_.begin(), children_.end(), 
    [&](GameObject* ch)
    {
        return ch == &child;
    });

    if (it != children_.end())
    {
        children_.erase(it, children_.end());
        child.removeParent(*this);
    }
}

bool GameObject::hasChildren() const
{
    return !children_.empty();
}

bool GameObject::isAChildOf(const GameObject& parent) const
{
    if (this == &parent)
    {
        return false;
    }

    auto it = std::find(parent.children_.begin(), parent.children_.end(), this);
    return it != parent.children_.end();
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
        data_.model = parent_ ? parent_->data_.model * transform_->getModelMatrix() : transform_->getModelMatrix();
        data_.position = transform_->position;
    }
}

}