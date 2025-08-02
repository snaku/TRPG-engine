#pragma once

#include "Include/GameObjectData.hpp"
#include "Include/Vertex.hpp"
#include <vector>

struct RenderableData
{
    const GameObjectData* gameObjectData = nullptr;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};