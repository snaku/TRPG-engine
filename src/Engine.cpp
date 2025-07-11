#include "Include/Window.hpp"
#include "Include/VulkanRenderer.hpp"
#include "Include/Engine.hpp"

#include <memory>

namespace engine
{
    static Window window;
    static std::unique_ptr<Renderer> renderer;
    
    /*
    *  initialize the vulkan instance, device, swapchain,
    *  render pass
    */
    void initEngine()
    {
        renderer = std::make_unique<VulkanRenderer>(window);
        
        renderer->init();
    }

    // create the graphics pipeline
    void createPipeline(const std::string& vertexShader, const std::string& fragmentShader)
    {
        renderer->createPipeline(vertexShader, fragmentShader);
    }

    // create a simple game object
    void createObject()
    {
        renderer->createObject();
    }

    void prepareRender()
    {
        renderer->prepareRender();
    }

    void render()
    {
        renderer->renderFrame();
    }

    // initialize the window
    bool initWindow(uint32_t width, uint32_t height, const std::string& title)
    {
        return window.createWindow(width, height, title);
    }
    
    // condition for the main loop = while (!engine::windowShouldClose())
    bool windowShouldClose()
    {
        return window.windowShouldClose();
    }

    // put inside the main loop
    void pollEvents()
    {
        window.pollEvents();
    }
}