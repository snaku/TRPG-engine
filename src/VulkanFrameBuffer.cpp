#include "../Include/VulkanContext.hpp"
#include "../Include/VulkanSwapchain.hpp"
#include "../Include/VulkanFrameBuffer.hpp"

VulkanFrameBuffer::VulkanFrameBuffer(VulkanContext& ctx, const VulkanSwapchain& vkSwapchain) 
    : vkCtx_(ctx), vkSwapchain_(vkSwapchain)
{
    createFrameBuffers();
}
VulkanFrameBuffer::~VulkanFrameBuffer() noexcept
{
    for (auto framebuffer : vkCtx_.swapChainFrameBuffers)
    {
        vkDestroyFramebuffer(vkCtx_.device, framebuffer, nullptr);
    }
}

void VulkanFrameBuffer::createFrameBuffers()
{
    vkCtx_.swapChainFrameBuffers.resize(vkCtx_.swapchainImageViews.size());

    for (std::size_t i = 0; i < vkCtx_.swapChainFrameBuffers.size(); i++)
    {
            VkImageView attachments[] = 
            {
                vkCtx_.swapchainImageViews[i]
            };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = vkCtx_.renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = vkSwapchain_.getExtent().width;
        framebufferInfo.height = vkSwapchain_.getExtent().height;
        framebufferInfo.layers = 1;

        VK_CHECK(vkCreateFramebuffer(vkCtx_.device, &framebufferInfo, nullptr, &vkCtx_.swapChainFrameBuffers[i]));
    }
}