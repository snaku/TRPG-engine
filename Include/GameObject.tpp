#include "Include/GameObject.hpp"

template<typename T>
T* engine::GameObject::getComponent()
{
    if constexpr (!std::is_base_of<IComponent, T>::value)
    {
        std::cerr << "[engine::GameObject::getComponent !ERROR!]  \"" << name_
                  << "\" component of type " 
                  << typeid(T).name() 
                  << " does not inherit from IComponent.\n";
        return nullptr;
    }

    if (!hasComponent<T>())
    {
        std::cerr << "[engine::GameObject::getComponent !ERROR!]: \"" << name_ 
                  << "\" does not have component of type " 
                  << typeid(T).name() << ".\n";
        return nullptr;
    }

    T* pComp = nullptr;
    for (const auto& component : components_)
    {
        if (component && !pComp)
        {
            pComp = dynamic_cast<T*>(component.get());
        }

        if (pComp)
        {
            return pComp;
        }
    }

    return nullptr;
}

template<typename T, typename... Args>
T* engine::GameObject::addComponent(Args&&... args)
{
    if constexpr (!std::is_base_of<IComponent, T>::value)
    {
        std::cerr << "[engine::GameObject::addComponent !ERROR!]  \"" << name_
                  << "\" component of type " 
                  << typeid(T).name() 
                  << " does not inherit from IComponent.\n";
        return nullptr;
    }

    if (hasComponent<T>())
    {
        std::cerr << "[engine::GameObject::addComponent !ERROR!]  \"" << name_
                  << "\" already has component of type "
                  << typeid(T).name() << ".\n";
        return nullptr;
    }

    auto component = std::make_shared<T>(std::forward<Args>(args)...);
    T* pComp = component.get();

    components_.push_back(std::move(component));
    componentTypes_.insert(typeid(T));

    return pComp;
}

template<typename T>
bool engine::GameObject::hasComponent()
{
    return componentTypes_.find(typeid(T)) != componentTypes_.end();
}

template<typename T>
void engine::GameObject::removeComponent()
{
    if (!hasComponent<T>())
    {
        std::cerr << "[engine::GameObject::removeComponent !ERROR!]  \"" << name_
                  << "\" does not have component of type " 
                  << typeid(T).name() << ".\n";
        return;
    }

    // unordered_set
    componentTypes_.erase(typeid(T));

    // vector
    auto it = std::remove_if(components_.begin(), components_.end(), 
    [](const std::shared_ptr<IComponent>& c) -> bool
    {
        // return true if c can be cast to T
        return dynamic_cast<T*>(c.get());
    });
    components_.erase(it, components_.end());
}