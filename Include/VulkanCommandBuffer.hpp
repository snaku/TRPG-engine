#pragma once

#include <memory>

struct VulkanContext;
struct RenderableData;
class VulkanDevice;
class VulkanSwapchain;
class VulkanMesh;

class VulkanCommandBuffer
{
public:    
    VulkanCommandBuffer(VulkanContext& ctx, const VulkanDevice& vkDevice,
        const VulkanSwapchain& vkSwapchain, const std::vector<std::unique_ptr<VulkanMesh>>& meshes);
    
    ~VulkanCommandBuffer() noexcept;

    void drawFrame(const std::vector<RenderableData>& renderables, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
private:
    void createCommandPool();
    void recordCommandBuffer(uint32_t imgIndex, const std::vector<RenderableData>& renderables, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
    void allocateCommandBuffers();
    void createSemaphore();
    void createFence();

    VulkanContext& vkCtx_;
    const VulkanDevice& vkDevice_;
    const VulkanSwapchain& vkSwapchain_;
    const std::vector<std::unique_ptr<VulkanMesh>>& meshes_;
    const int MAX_FRAMES_IN_FLIGHT = 2;
    VkFence inFlightFence = VK_NULL_HANDLE;
};