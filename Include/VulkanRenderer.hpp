#pragma once

#include "Include/VulkanContext.hpp"
#include "Include/VulkanInstance.hpp"
#include "Include/VulkanDevice.hpp"
#include "Include/VulkanSwapchain.hpp"
#include "Include/VulkanRenderPass.hpp"
#include "Include/Vertex.hpp"
#include "Include/VulkanPipeline.hpp"
#include "Include/VulkanFrameBuffer.hpp"
#include "Include/VulkanTexture.hpp"
#include "Include/VulkanMesh.hpp"
#include "Include/VulkanCommandBuffer.hpp"
#include "Include/Renderer.hpp"

class Window;

class VulkanRenderer : public Renderer
{
public:
    explicit VulkanRenderer(Window& window);

    void init() override;
    void createPipeline(std::string_view vertexShader, std::string_view fragmentShader) override;
    void prepareRender() override;
    void createObject() override;
    void createMesh(const RenderableData& data) override;
    void renderFrame(const std::vector<RenderableData>& renderables, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;
    void loadTexture(const std::filesystem::path& texturePath) override;

private:
    Window& window_;

    VulkanContext vkCtx_;
    std::unique_ptr<VulkanInstance> vkInstance_;
    std::unique_ptr<VulkanDevice> vkDevice_;
    std::unique_ptr<VulkanSwapchain> vkSwapchain_;
    std::unique_ptr<VulkanRenderPass> vkRenderPass_;
    std::unique_ptr<VulkanPipeline> vkPipeline_;
    std::unique_ptr<VulkanFrameBuffer> vkFramebuffer_;
    std::unique_ptr<VulkanTexture> vkTexture_;
    std::vector<std::unique_ptr<VulkanMesh>> vkMeshes_;
    std::unique_ptr<VulkanCommandBuffer> vkCmdBuffer_;
};