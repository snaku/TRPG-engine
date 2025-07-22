#include "Include/Window.hpp"
#include "Include/Camera.hpp"
#include "Include/VulkanRenderer.hpp"
#include "Include/Engine.hpp"

#include <memory>

namespace engine
{
    namespace
    {
        Window window;
        std::unique_ptr<Renderer> renderer;
    }

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
    void createPipeline(std::string_view vertexShader, std::string_view fragmentShader)
    {
        renderer->createPipeline(vertexShader, fragmentShader);
    }

    void loadTexture(const std::filesystem::path& texturePath)
    {
        renderer->loadTexture(texturePath);
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

    void render(const Camera& camera)
    {
        renderer->renderFrame(camera);
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

    int right()
    {
        return glfwGetKey(window.getWindow(), GLFW_KEY_D);
    }
    int left()
    {
        return glfwGetKey(window.getWindow(), GLFW_KEY_A);
    }
    int up()
    {
        return glfwGetKey(window.getWindow(), GLFW_KEY_W);
    }
    int down()
    {
        return glfwGetKey(window.getWindow(), GLFW_KEY_S);
    }
}