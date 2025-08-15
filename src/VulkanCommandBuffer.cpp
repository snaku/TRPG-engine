#include "Include/VulkanContext.hpp"
#include "Include/VulkanDevice.hpp"
#include "Include/VulkanSwapchain.hpp"
#include "Include/VulkanMesh.hpp"
#include "Include/RenderableData.hpp"
#include "Include/VulkanCommandBuffer.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

VulkanCommandBuffer::VulkanCommandBuffer(VulkanContext& ctx,
                                        const VulkanDevice& vkDevice,
                                        const VulkanSwapchain& vkSwapchain,
                                        const std::vector<std::unique_ptr<VulkanMesh>>& meshes) 
    : vkCtx_(ctx), vkDevice_(vkDevice), vkSwapchain_(vkSwapchain), meshes_(meshes)
{
    createCommandPool();
    allocateCommandBuffers();
    createSemaphore();
    createFence();
}
VulkanCommandBuffer::~VulkanCommandBuffer() noexcept
{
    vkDeviceWaitIdle(vkCtx_.device);

    if (vkCtx_.commandPool != VK_NULL_HANDLE)
	{
		vkDestroyCommandPool(vkCtx_.device, vkCtx_.commandPool, nullptr);
	}
    if (vkCtx_.renderFinishedSemaphore != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(vkCtx_.device, vkCtx_.renderFinishedSemaphore, nullptr);
    }
    if (vkCtx_.imgAvailableSemaphore != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(vkCtx_.device, vkCtx_.imgAvailableSemaphore, nullptr);
    }
    if (inFlightFence != VK_NULL_HANDLE)
    {
        vkDestroyFence(vkCtx_.device, inFlightFence, nullptr);
    }
}

void VulkanCommandBuffer::recordCommandBuffer(uint32_t imgIndex, const std::vector<RenderableData>& renderables, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
    VK_CHECK(vkResetCommandBuffer(vkCtx_.commandBuffers[imgIndex], 0));

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VK_CHECK(vkBeginCommandBuffer(vkCtx_.commandBuffers[imgIndex], &beginInfo));

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = vkCtx_.renderPass;
    renderPassInfo.framebuffer = vkCtx_.swapChainFrameBuffers[imgIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = vkSwapchain_.getExtent();
        
    VkClearValue clearColor = {{{0.0f, 0.1f, 0.1f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(vkCtx_.commandBuffers[imgIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(vkCtx_.commandBuffers[imgIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, vkCtx_.pipeline);
    vkCmdBindDescriptorSets(vkCtx_.commandBuffers[imgIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, vkCtx_.pipelineLayout, 0, 1, &vkCtx_.descriptorSet, 0, nullptr);

    for (std::size_t i = 0; i < renderables.size(); ++i)
    {
        const auto& renderable = renderables[i];
        const auto& mesh = meshes_[i];

        mesh->updatePushConstantData(renderable.gameObjectData->model);
        mesh->updateUniformBuffer(viewMatrix, projectionMatrix);

        VkDeviceSize offsets[] = {0};
        
        mesh->bind(vkCtx_.commandBuffers[imgIndex]);
        mesh->sendPushConstantData(vkCtx_.commandBuffers[imgIndex], vkCtx_.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4));
        vkCmdDrawIndexed(vkCtx_.commandBuffers[imgIndex], (uint32_t)mesh->getIndices().size(), 1, 0, 0, 0);
    }
     
    vkCmdEndRenderPass(vkCtx_.commandBuffers[imgIndex]);

    VK_CHECK(vkEndCommandBuffer(vkCtx_.commandBuffers[imgIndex]));
}

void VulkanCommandBuffer::allocateCommandBuffers()
{
    vkCtx_.commandBuffers.resize(vkCtx_.swapChainFrameBuffers.size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = vkCtx_.commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)vkCtx_.commandBuffers.size();

    VK_CHECK(vkAllocateCommandBuffers(vkCtx_.device, &allocInfo, vkCtx_.commandBuffers.data()));
}

void VulkanCommandBuffer::createSemaphore()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VK_CHECK(vkCreateSemaphore(vkCtx_.device, &semaphoreInfo, nullptr, &vkCtx_.imgAvailableSemaphore));
    VK_CHECK(vkCreateSemaphore(vkCtx_.device, &semaphoreInfo, nullptr, &vkCtx_.renderFinishedSemaphore));
}

void VulkanCommandBuffer::createFence()
{
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VK_CHECK(vkCreateFence(vkCtx_.device, &fenceInfo, nullptr, &inFlightFence));
}

void VulkanCommandBuffer::drawFrame(const std::vector<RenderableData>& renderables, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
    VK_CHECK(vkWaitForFences(vkCtx_.device, 1, &inFlightFence, VK_TRUE, UINT64_MAX));

    if (inFlightFence != VK_NULL_HANDLE)
    {
        VK_CHECK(vkResetFences(vkCtx_.device, 1, &inFlightFence));
    }

    uint32_t imgIndex;
    vkAcquireNextImageKHR(vkCtx_.device, vkCtx_.swapchain, UINT64_MAX, vkCtx_.imgAvailableSemaphore, VK_NULL_HANDLE, &imgIndex);
    
    recordCommandBuffer(imgIndex, renderables, viewMatrix, projectionMatrix);
    
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

    VK_CHECK(vkQueueSubmit(vkCtx_.graphicsQueue, 1, &submitInfo, inFlightFence));

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {vkCtx_.swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imgIndex;

    VK_CHECK(vkQueuePresentKHR(vkCtx_.presentQueue, &presentInfo));
}

void VulkanCommandBuffer::createCommandPool()
{
	VkCommandPoolCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	createInfo.queueFamilyIndex = vkDevice_.getGraphicsFamilyIndices();
	createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	VK_CHECK(vkCreateCommandPool(vkCtx_.device, &createInfo, nullptr, &vkCtx_.commandPool));
}