#pragma once

#include "Include/Camera.hpp"
#include "Include/Scene.hpp"
#include <string>
#include <filesystem>

struct RenderableData;

namespace engine
{
    void initEngine();
    void createPipeline(std::string_view vertexShader, std::string_view fragmentShader);
    void loadTexture(const std::filesystem::path& texturePath);
    void createCamera();
    void createObject();
    void createMesh(const RenderableData& data);
    void prepareRender();
    void render(const Scene& scene, const Camera& camera);

    bool initWindow(uint32_t width, uint32_t height, const std::string& title);
    bool windowShouldClose();
    void pollEvents();
    
    // TEST INPUT: keyboard keys
    int right();
    int left();
    int up();
    int down();

    // arrow keys
    int downArrow();
    int upArrow();
    int leftArrow();
    int rightArrow();

    int shiftKey();
}