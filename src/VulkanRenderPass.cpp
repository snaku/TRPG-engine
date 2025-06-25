#include "../Include/VulkanContext.hpp"
#include "../Include/VulkanSwapchain.hpp"
#include "../Include/VulkanRenderPass.hpp"

VulkanRenderPass::VulkanRenderPass(VulkanContext& ctx, const VulkanSwapchain& vkSwapchain)
    : vkCtx_(ctx), vkSwapchain_(vkSwapchain)
{
    createRenderPass();
}
VulkanRenderPass::~VulkanRenderPass() noexcept
{
    if (vkCtx_.renderPass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(vkCtx_.device, vkCtx_.renderPass, nullptr);
    }
}

void VulkanRenderPass::createRenderPass()
{
    VkFormat format = vkSwapchain_.getFormat();

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    createInfo.attachmentCount = 1;
    createInfo.pAttachments = &colorAttachment;
    createInfo.subpassCount = 1;
    createInfo.pSubpasses = &subpass;

    VK_CHECK(vkCreateRenderPass(vkCtx_.device, &createInfo, nullptr, &vkCtx_.renderPass));
}