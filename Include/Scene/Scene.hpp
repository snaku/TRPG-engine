#pragma once

#include <Scene/RenderableData.hpp>
#include <Scene/ShapeType.hpp>

// std
#include <string>
#include <vector>
#include <memory>

namespace engine
{

class GameObject;

class Scene
{
public:
    GameObject& createGameObject(std::string_view name, ShapeType shape);

    void update(float deltaTime);

    const std::vector<std::unique_ptr<GameObject>>& getGameObjects() const { return objects_; };
    const std::vector<RenderableData>& getRenderableData() const { return renderables_; }
private:
    std::vector<RenderableData> renderables_;
    std::vector<std::unique_ptr<GameObject>> objects_;
};

}