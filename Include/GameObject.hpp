#pragma once

#include "Include/GameObjectData.hpp"

#include <string>
#include <vector>
#include <memory>
#include <unordered_set>
#include <typeindex>
#include <algorithm>
#include <iostream>

struct IComponent;

namespace engine
{

class GameObject
{
public:
    GameObject(std::string name, float xPos, float yPos, float zPos);

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
    glm::vec3 getPosition() const { return pos_; }
private:
    GameObjectData data_;

    std::vector<std::shared_ptr<::IComponent>> components_;
    std::unordered_set<std::type_index> componentTypes_;
    std::string name_;
    glm::mat4 model_;
    glm::vec3 pos_;
};

}

#include "Include/GameObject.tpp"