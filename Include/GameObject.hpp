#pragma once

#include "Include/GameObjectData.hpp"

#include <string>
#include <vector>
#include <memory>

struct IComponent;

namespace engine
{

class GameObject
{
public:
    GameObject(std::string name, float xPos, float yPos, float zPos);

    /*template<typename T, typename... Args>
    T* addComponent(Args&&... args);*/

    template<typename T, typename... Args>
    T* addComponent(Args&&... args)
    {
        if constexpr (!std::is_base_of<IComponent, T>::value)
        {
            return nullptr;
        }

        auto component = std::make_shared<T>(std::forward<Args>(args)...);
        T* pComp = component.get();
        components_.push_back(std::move(component));

        return pComp;
    }

    void update(float deltaTime);

    // getters
    template<typename T>
    T& getComponent();
    const GameObjectData& getData() const { return data_; }
    glm::vec3 getPosition() const { return pos_; }
private:
    GameObjectData data_;
    std::vector<std::shared_ptr<::IComponent>> components_;
    std::string name_;
    glm::mat4 model_;
    glm::vec3 pos_;
};

}