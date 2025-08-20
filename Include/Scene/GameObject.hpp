#pragma once

#include <Scene/GameObjectData.hpp>

// std
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>
#include <typeindex>
#include <algorithm>
#include <iostream>

struct IComponent;
struct TransformComponent;

namespace engine
{

class GameObject
{
public:
    GameObject(const std::string& name);

    template<typename T, typename... Args>
    T* addComponent(Args&&... args);

    template<typename T>
    T* getComponent();

    template<typename T>
    bool hasComponent();

    template<typename T>
    void removeComponent();

    void update(float deltaTime);

    // getters
    const GameObjectData& getData() const { return data_; }
    const std::string& getName() const { return name_; }
private:
    GameObjectData data_;

    std::vector<std::shared_ptr<::IComponent>> components_;
    std::unordered_set<std::type_index> componentTypes_;
    std::string name_;

    TransformComponent* transform_ = nullptr;
};

}

#include <Scene/GameObject.tpp>