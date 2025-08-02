#pragma once

#include <string>
#include <filesystem>

class Camera;
struct RenderableData;

namespace engine
{
    class Scene;
}

class Renderer
{
public:
    virtual ~Renderer() = default;
    
    virtual void init() = 0;
    virtual void createPipeline(std::string_view vertexShader, std::string_view fragmentShader) = 0;
    virtual void prepareRender() = 0;
    virtual void createObject() = 0;
    virtual void createMesh(const RenderableData& data) = 0;
    virtual void renderFrame(const engine::Scene& scene, const Camera& camera) = 0;
    virtual void loadTexture(const std::filesystem::path& texturePath) = 0;
};