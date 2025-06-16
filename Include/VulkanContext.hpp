#pragma once
#include <vulkan/vulkan.h>
#include <stdexcept>

#define VK_THROW(msg) throw std::runtime_error(std::string("Vulkan error: ") + (msg))
#define VK_CHECK(x)															\
	do {																	\
		VkResult error = (x);												\
		if (error != VK_SUCCESS) VK_THROW(#x);								\
	} while (0)							

struct VulkanContext
{
	VkInstance instance = VK_NULL_HANDLE;
};