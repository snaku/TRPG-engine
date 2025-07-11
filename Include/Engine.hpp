#pragma once

#include <string>

namespace engine
{
    void initEngine();
    void createPipeline(const std::string& vertexShader, const std::string& fragmentShader);
    void createObject();
    void prepareRender();
    void render();

    bool initWindow(uint32_t width, uint32_t height, const std::string& title);
    bool windowShouldClose();
    void pollEvents();
}