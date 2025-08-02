#include "Include/GameObject.hpp"
#include "Include/Scene.hpp"

namespace engine
{

GameObject& Scene::createGameObject(std::string_view name)
{
    std::vector<Vertex> vertices = 
    {
        {{-0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}}, // 0
        {{0.0f, 0.5f},   {1.0f, 1.0f, 1.0f}}, // 1
        {{0.5f, -0.5f},  {1.0f, 1.0f, 1.0f}}, // 2
        {{1.0f, 0.5f},   {1.0f, 1.0f, 1.0f}}, // 3
    };

    std::vector<uint32_t> indices = { 0, 1, 2, 2, 1, 3 };

    auto gameObject = std::make_unique<GameObject>(std::string{name}, 0, 0, 0);

    RenderableData renderable;
    renderable.gameObjectData = &gameObject->getData();
    renderable.vertices = std::move(vertices);
    renderable.indices = std::move(indices);

    renderables_.push_back(std::move(renderable));
    objects_.push_back(std::move(gameObject));

    return *objects_.back();
} 

void Scene::update(float deltaTime)
{
    for (const auto& object : objects_)
    {
        if (object)
        {
            object->update(deltaTime);
        }
    }
}

}