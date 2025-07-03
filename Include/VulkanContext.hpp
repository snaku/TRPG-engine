#pragma once
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>

#define VK_THROW(msg) throw std::runtime_error(std::string("Vulkan error: ") + (msg))
#define VK_CHECK(x)															\
	do {																	\
		VkResult error = (x);												\
		if (error != VK_SUCCESS) VK_THROW(#x);								\
	} while (0)							

struct VulkanContext
{
	VkInstance instance = VK_NULL_HANDLE;
	VkSurfaceKHR surface = VK_NULL_HANDLE;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;

	// queue
	VkQueue graphicsQueue = VK_NULL_HANDLE;
	VkQueue presentQueue = VK_NULL_HANDLE;

	// swapchain
	VkSwapchainKHR swapchain = VK_NULL_HANDLE;
	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapchainImageViews;

	VkRenderPass renderPass = VK_NULL_HANDLE;

	// pipeline
	VkPipeline pipeline = VK_NULL_HANDLE;
	VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;

	std::vector<VkFramebuffer> swapChainFrameBuffers;

	VkCommandPool commandPool = VK_NULL_HANDLE;
	std::vector<VkCommandBuffer> commandBuffers;

	// semaphores
	VkSemaphore imgAvailableSemaphore = VK_NULL_HANDLE;
	VkSemaphore renderFinishedSemaphore = VK_NULL_HANDLE;

	VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
	VkDescriptorSet descriptorSet;

	// validation layers
	std::vector<const char*> validationLayers;
	bool enableValidationLayers;
};