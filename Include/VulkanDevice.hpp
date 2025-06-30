#pragma once

#include <vector>
#include <optional>

struct VulkanContext;

class VulkanDevice
{
public:
	explicit VulkanDevice(VulkanContext& ctx);
	~VulkanDevice() noexcept;

	uint32_t getGraphicsFamilyIndices() const { return indices_.graphicsFamily.value(); }
	uint32_t getPresentFamilyIndices() const { return indices_.presentFamily.value(); }



private: 
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() const
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};	

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device);
	void pickPhysicalDevice();
	void createLogicalDevice();
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	VulkanContext& vkCtx_;
	QueueFamilyIndices indices_;
	std::vector<const char*> deviceExtension_{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
};