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

	VkSwapchainKHR swapchain = VK_NULL_HANDLE;
	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapchainImageViews;

	VkCommandPool commandPool = VK_NULL_HANDLE;

	// validation layers
	std::vector<const char*> validationLayers;
	bool enableValidationLayers;
};