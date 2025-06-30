#include "../Include/VulkanContext.hpp"
#include "../Include/VulkanDevice.hpp"
#include "../Include/VulkanSwapchain.hpp"
#include "../Include/VulkanMesh.hpp"
#include "../Include/VulkanCommandBuffer.hpp"

#include <GLFW/glfw3.h>

VulkanCommandBuffer::VulkanCommandBuffer(VulkanContext& ctx,
                                        const VulkanDevice& vkDevice,
                                        const VulkanSwapchain& vkSwapchain,
                                        const std::vector<std::unique_ptr<VulkanMesh>>& meshes) 
    : vkCtx_(ctx), vkDevice_(vkDevice), vkSwapchain_(vkSwapchain), meshes_(meshes)
{
    createCommandPool();
    createCommandBuffer();
    createSemaphore();
}
VulkanCommandBuffer::~VulkanCommandBuffer() noexcept
{
    if (vkCtx_.commandPool != VK_NULL_HANDLE)
	{
		vkDestroyCommandPool(vkCtx_.device, vkCtx_.commandPool, nullptr);
	}
    vkDestroySemaphore(vkCtx_.device, vkCtx_.renderFinishedSemaphore, nullptr);
    vkDestroySemaphore(vkCtx_.device, vkCtx_.imgAvailableSemaphore, nullptr);
}

void VulkanCommandBuffer::createCommandBuffer()
{
    vkCtx_.commandBuffers.resize(vkCtx_.swapChainFrameBuffers.size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = vkCtx_.commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)vkCtx_.commandBuffers.size();

    VK_CHECK(vkAllocateCommandBuffers(vkCtx_.device, &allocInfo, vkCtx_.commandBuffers.data()));

    for (std::size_t i = 0; i < vkCtx_.commandBuffers.size(); i++)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        VK_CHECK(vkBeginCommandBuffer(vkCtx_.commandBuffers[i], &beginInfo));

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = vkCtx_.renderPass;
        renderPassInfo.framebuffer = vkCtx_.swapChainFrameBuffers[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = vkSwapchain_.getExtent();
        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(vkCtx_.commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(vkCtx_.commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vkCtx_.pipeline);
        
        for (const auto& mesh : meshes_)
        {
            VkBuffer vertexBuffers[] = { mesh->getVertexBuffer() };
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(vkCtx_.commandBuffers[i], 0, 1, vertexBuffers, offsets);
            vkCmdBindDescriptorSets(vkCtx_.commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vkCtx_.pipelineLayout, 0, 1, &vkCtx_.descriptorSet, 0, nullptr);
            vkCmdDraw(vkCtx_.commandBuffers[i], (uint32_t)mesh->getVerticesSize(), 1, 0, 0);
        }
        
        vkCmdEndRenderPass(vkCtx_.commandBuffers[i]);

        VK_CHECK(vkEndCommandBuffer(vkCtx_.commandBuffers[i]));
    }
}

void VulkanCommandBuffer::createSemaphore()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VK_CHECK(vkCreateSemaphore(vkCtx_.device, &semaphoreInfo, nullptr, &vkCtx_.imgAvailableSemaphore));
    VK_CHECK(vkCreateSemaphore(vkCtx_.device, &semaphoreInfo, nullptr, &vkCtx_.renderFinishedSemaphore));
}

void VulkanCommandBuffer::drawFrame()
{
    uint32_t imgIndex;
    vkAcquireNextImageKHR(vkCtx_.device, vkCtx_.swapchain, UINT64_MAX, vkCtx_.imgAvailableSemaphore, VK_NULL_HANDLE, &imgIndex);

    // deltatime here
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastTime_;
    lastTime_ = currentTime;

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {vkCtx_.imgAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &vkCtx_.commandBuffers[imgIndex];

    VkSemaphore signalSemaphores[] = {vkCtx_.renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    for (auto& mesh : meshes_)
    {
        mesh->updateUniformBuffer(deltaTime);
    }

    VK_CHECK(vkQueueSubmit(vkCtx_.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE));

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {vkCtx_.swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imgIndex;

    vkQueuePresentKHR(vkCtx_.presentQueue, &presentInfo);
}

void VulkanCommandBuffer::createCommandPool()
{
	VkCommandPoolCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	createInfo.queueFamilyIndex = vkDevice_.getGraphicsFamilyIndices();
	createInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

	VK_CHECK(vkCreateCommandPool(vkCtx_.device, &createInfo, nullptr, &vkCtx_.commandPool));
}