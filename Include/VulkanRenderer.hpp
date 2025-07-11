#pragma once

#include "Include/VulkanContext.hpp"
#include "Include/VulkanInstance.hpp"
#include "Include/VulkanDevice.hpp"
#include "Include/VulkanSwapchain.hpp"
#include "Include/VulkanRenderPass.hpp"
#include "Include/Vertex.hpp"
#include "Include/VulkanPipeline.hpp"
#include "Include/VulkanFrameBuffer.hpp"
#include "Include/VulkanMesh.hpp"
#include "Include/VulkanCommandBuffer.hpp"
#include "Include/Renderer.hpp"

class Window;

class VulkanRenderer : public Renderer
{
public:
    VulkanRenderer(Window& window);

    void init() override;
    void createPipeline(const std::string& vertexShader, const std::string& fragmentShader) override;
    void prepareRender() override;
    void createObject() override;
    void renderFrame() override;

private:
    Window& window_;

    VulkanContext vkCtx_;
    std::unique_ptr<VulkanInstance> vkInstance_;
    std::unique_ptr<VulkanDevice> vkDevice_;
    std::unique_ptr<VulkanSwapchain> vkSwapchain_;
    std::unique_ptr<VulkanRenderPass> vkRenderPass_;
    std::unique_ptr<VulkanPipeline> vkPipeline_;
    std::unique_ptr<VulkanFrameBuffer> vkFramebuffer_;
    std::vector<std::unique_ptr<VulkanMesh>> vkMeshes_;
    std::unique_ptr<VulkanCommandBuffer> vkCmdBuffer_;
};