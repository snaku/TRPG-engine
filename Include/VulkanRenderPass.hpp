#pragma once

struct VulkanContext;
class VulkanSwapchain;

class VulkanRenderPass
{
public:
    VulkanRenderPass(VulkanContext& ctx, const VulkanSwapchain& vkSwapchain);
    ~VulkanRenderPass() noexcept;

private:
    void createRenderPass();

    VulkanContext& vkCtx_;
    const VulkanSwapchain& vkSwapchain_;
};