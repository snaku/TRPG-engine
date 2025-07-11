#pragma once

#include <string>

class Renderer
{
public:
    virtual ~Renderer() = default;

    virtual void init() = 0;
    virtual void createPipeline(const std::string& vertexShader, const std::string& fragmentShader) = 0;
    virtual void prepareRender() = 0;
    virtual void createObject() = 0;
    virtual void renderFrame() = 0;
};