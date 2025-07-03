#include "../Include/Window.hpp"
#include "../Include/VulkanContext.hpp"
#include "../Include/VulkanInstance.hpp"
#include "../Include/VulkanDevice.hpp"
#include "../Include/VulkanSwapchain.hpp"
#include "../Include/VulkanRenderPass.hpp"
#include "../Include/Vertex.hpp"
#include "../Include/VulkanPipeline.hpp"
#include "../Include/VulkanFrameBuffer.hpp"
#include "../Include/VulkanMesh.hpp"
#include "../Include/VulkanCommandBuffer.hpp"
#include "../Include/Engine.hpp"

#include <memory>
#include <cassert>

namespace engine
{
    namespace
    {
        Window window;
        VulkanContext ctx;

        std::unique_ptr<VulkanInstance> vkInstance;
        std::unique_ptr<VulkanDevice> vkDevice;
        std::unique_ptr<VulkanSwapchain> vkSwapchain;
        std::unique_ptr<VulkanRenderPass> vkRenderPass;
        std::unique_ptr<VulkanPipeline> vkPipeline;
        std::unique_ptr<VulkanFrameBuffer> vkFramebuffer;
        std::vector<std::unique_ptr<VulkanMesh>> vkMeshes;
        std::unique_ptr<VulkanCommandBuffer> vkCmdBuffer;
    }

    /*
    *  initialize the vulkan instance,device, swapchain,
    *  render pass and vulkanMesh (vertex buffer and uniform buffer)
    */
    void initEngine()
    {
        vkInstance = std::make_unique<VulkanInstance>(ctx, window);
        vkDevice = std::make_unique<VulkanDevice>(ctx);
        vkSwapchain = std::make_unique<VulkanSwapchain>(ctx, *vkDevice, window);
        vkRenderPass = std::make_unique<VulkanRenderPass>(ctx, *vkSwapchain);

        std::vector<Vertex> vertices = 
        {
            {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}}, // 0
            {{0.0f, 0.5f},   {0.0f, 1.0f, 0.0f}}, // 1
            {{0.5f, -0.5f},  {0.0f, 0.0f, 1.0f}}, // 2
            {{1.0f, 0.5f},   {1.0f, 1.0f, 0.0f}}, // 3
        };
        std::vector<uint32_t> indices = { 0, 1, 2, 2, 1, 3 };
        
        vkMeshes.emplace_back(std::make_unique<VulkanMesh>(ctx, *vkSwapchain, vertices, indices));
    }

    // create the graphics pipeline
    void createPipeline(const std::string& vertexShader, const std::string& fragmentShader)
    {
        assert(vkSwapchain != nullptr);

        vkPipeline = std::make_unique<VulkanPipeline>(ctx, *vkSwapchain, vkMeshes);
        vkPipeline->createShader(vertexShader, fragmentShader);
        vkPipeline->createPipeline();
    }

    void prepareRender()
    {
        assert(vkSwapchain != nullptr && vkDevice != nullptr);
        
        vkFramebuffer = std::make_unique<VulkanFrameBuffer>(ctx, *vkSwapchain);
        vkCmdBuffer = std::make_unique<VulkanCommandBuffer>(ctx, *vkDevice, *vkSwapchain, vkMeshes);
    }

    void render()
    {
        assert(vkCmdBuffer != nullptr);

        vkCmdBuffer->drawFrame();
        vkDeviceWaitIdle(ctx.device); // will do better later
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