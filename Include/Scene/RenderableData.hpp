#pragma once

#include <Scene/GameObjectData.hpp>
#include <Renderer/VulkanRenderer/Vertex.hpp>

// std
#include <vector>

struct RenderableData
{
    const GameObjectData* gameObjectData = nullptr;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};