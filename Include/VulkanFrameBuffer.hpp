#pragma once

struct VulkanContext;
class VulkanSwapchain;

class VulkanFrameBuffer
{
public:
    VulkanFrameBuffer(VulkanContext& ctx, const VulkanSwapchain& vkSwapchain);
    ~VulkanFrameBuffer() noexcept;

    void createFrameBuffers();
private:
    VulkanContext& vkCtx_;
    const VulkanSwapchain& vkSwapchain_;
};