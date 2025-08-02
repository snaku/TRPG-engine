#pragma once

#include <memory>

struct VulkanContext;
class VulkanDevice;
class VulkanSwapchain;
class Camera;
class VulkanMesh;

namespace engine
{
    class Scene;
}

class VulkanCommandBuffer
{
public:    
    VulkanCommandBuffer(VulkanContext& ctx, const VulkanDevice& vkDevice,
        const VulkanSwapchain& vkSwapchain, const std::vector<std::unique_ptr<VulkanMesh>>& meshes);
    
    ~VulkanCommandBuffer() noexcept;

    void drawFrame(const engine::Scene& scene, const Camera& camera);
private:
    void createCommandPool();
    void createCommandBuffer();
    void createSemaphore();

    VulkanContext& vkCtx_;
    const VulkanDevice& vkDevice_;
    const VulkanSwapchain& vkSwapchain_;
    const std::vector<std::unique_ptr<VulkanMesh>>& meshes_;
    float lastTime_ = 0.0f; // temporary, will move that later
};