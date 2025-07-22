#pragma once

#include "Include/Camera.hpp"
#include <string>
#include <filesystem>

namespace engine
{
    void initEngine();
    void createPipeline(std::string_view vertexShader, std::string_view fragmentShader);
    void loadTexture(const std::filesystem::path& texturePath);
    void createCamera();
    void createObject();
    void prepareRender();
    void render(const Camera& camera);

    bool initWindow(uint32_t width, uint32_t height, const std::string& title);
    bool windowShouldClose();
    void pollEvents();
    int right();
    int left();
    int up();
    int down();
}