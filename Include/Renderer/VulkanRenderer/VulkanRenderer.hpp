#pragma once

#include <Renderer/Renderer.hpp>

#include <Renderer/VulkanRenderer/VulkanContext.hpp>
#include <Renderer/VulkanRenderer/VulkanInstance.hpp>
#include <Renderer/VulkanRenderer/VulkanDevice.hpp>
#include <Renderer/VulkanRenderer/VulkanSwapchain.hpp>
#include <Renderer/VulkanRenderer/VulkanRenderPass.hpp>
#include <Renderer/VulkanRenderer/Vertex.hpp>
#include <Renderer/VulkanRenderer/VulkanPipeline.hpp>
#include <Renderer/VulkanRenderer/VulkanFrameBuffer.hpp>
#include <Renderer/VulkanRenderer/VulkanTexture.hpp>
#include <Renderer/VulkanRenderer/VulkanMesh.hpp>
#include <Renderer/VulkanRenderer/VulkanCommandBuffer.hpp>

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