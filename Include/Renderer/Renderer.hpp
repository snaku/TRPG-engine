#pragma once

#include <glm/glm.hpp>

#include <string>
#include <filesystem>
#include <vector>

struct RenderableData;

class Renderer
{
public:
    virtual ~Renderer() = default;
    
    virtual void init() = 0;
    virtual void createPipeline(std::string_view vertexShader, std::string_view fragmentShader) = 0;
    virtual void prepareRender() = 0;
    virtual void createObject() = 0;
    virtual void createMesh(const RenderableData& data) = 0;
    virtual void renderFrame(const std::vector<RenderableData>& renderables, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) = 0;
    virtual void loadTexture(const std::filesystem::path& texturePath) = 0;
};