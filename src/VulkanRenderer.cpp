#include "Include/Window.hpp"
#include "Include/VulkanContext.hpp"
#include "Include/VulkanRenderer.hpp"

VulkanRenderer::VulkanRenderer(Window &window) 
    :  window_(window)
{
}

void VulkanRenderer::init()
{
    vkInstance_ = std::make_unique<VulkanInstance>(vkCtx_, window_);
    vkDevice_ = std::make_unique<VulkanDevice>(vkCtx_);
    vkSwapchain_ = std::make_unique<VulkanSwapchain>(vkCtx_, *vkDevice_, window_);
    vkRenderPass_ = std::make_unique<VulkanRenderPass>(vkCtx_, *vkSwapchain_);
}

void VulkanRenderer::createPipeline(std::string_view vertexShader, std::string_view fragmentShader)
{
    assert(vkSwapchain_ != nullptr);

    vkPipeline_ = std::make_unique<VulkanPipeline>(vkCtx_, *vkSwapchain_, vkMeshes_);
    vkPipeline_->createShader(vertexShader, fragmentShader);
    vkPipeline_->createPipeline();
}

void VulkanRenderer::prepareRender()
{
    assert(vkSwapchain_ != nullptr && vkDevice_ != nullptr);
        
    vkFramebuffer_ = std::make_unique<VulkanFrameBuffer>(vkCtx_, *vkSwapchain_);
    vkCmdBuffer_ = std::make_unique<VulkanCommandBuffer>(vkCtx_, *vkDevice_, *vkSwapchain_, vkMeshes_);
}

void VulkanRenderer::renderFrame(const Camera& camera)
{
    assert(vkCmdBuffer_ != nullptr);

    vkCmdBuffer_->drawFrame(camera);
    vkDeviceWaitIdle(vkCtx_.device); // will do better later
}

void VulkanRenderer::createObject()
{
    std::vector<Vertex> vertices = 
    {
        {{-0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}}, // 0
        {{0.0f, 0.5f},   {1.0f, 1.0f, 1.0f}}, // 1
        {{0.5f, -0.5f},  {1.0f, 1.0f, 1.0f}}, // 2
        {{1.0f, 0.5f},   {1.0f, 1.0f, 1.0f}}, // 3
    };
    std::vector<uint32_t> indices = { 0, 1, 2, 2, 1, 3 };
        
    vkMeshes_.emplace_back(std::make_unique<VulkanMesh>(vkCtx_, *vkSwapchain_, vertices, indices));
}

void VulkanRenderer::loadTexture(const std::filesystem::path& texturePath)
{
    vkTexture_ = std::make_unique<VulkanTexture>(vkCtx_);
    vkTexture_->createTextureImage(texturePath);
}