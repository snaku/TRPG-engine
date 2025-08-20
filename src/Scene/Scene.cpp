#include <Scene/GameObject.hpp>
#include <Scene/Scene.hpp>

namespace engine
{

GameObject& Scene::createGameObject(std::string_view name, ShapeType shape)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    switch (shape)
    {
        case ShapeType::PARALLELOGRAM_2D:
            vertices = 
            {
                {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
                {{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
                {{1.5f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
                {{0.5f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}
            };
            indices = 
            {
                0, 1, 2,
                2, 3, 0
            };
            break;
        case ShapeType::CUBE_3D:
            vertices = 
            {
                {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}}, // 0
                {{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}}, // 1
                {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}}, // 2
                {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}}, // 3

                {{-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}, // 4
                {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}, // 5
                {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}, // 6
                {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}, // 7
            };

            indices = 
            {
                0, 1, 2, 2, 3, 0,
                4, 6, 5, 6, 4, 7,
                4, 0, 3, 3, 7, 4,
                1, 5, 6, 6, 2, 1,
                3, 2, 6, 6, 7, 3,
                4, 5, 1, 1, 0, 4,
            };

            for (size_t i = 0; i < indices.size(); i += 3)
            {
                auto& v0 = vertices[indices[i + 0]];
                auto& v1 = vertices[indices[i + 1]];
                auto& v2 = vertices[indices[i + 2]];

                glm::vec3 edge1 = v1.pos - v0.pos;
                glm::vec3 edge2 = v2.pos - v0.pos;
                glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

                v0.normal += faceNormal;
                v1.normal += faceNormal;
                v2.normal += faceNormal;
            }

            for (auto& v : vertices)
            {
                v.normal = glm::normalize(v.normal);
            }
            break;
    }
    
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