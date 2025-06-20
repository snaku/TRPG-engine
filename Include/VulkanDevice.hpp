#pragma once

#include <optional>
#include <vector>

struct VulkanContext;

class VulkanDevice
{
public:
	explicit VulkanDevice(VulkanContext& ctx);
	~VulkanDevice() noexcept;

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() const
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

private:
	void pickPhysicalDevice();
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	void createLogicalDevice();
	void createCommandPool();

	VulkanContext& vkCtx_;
	std::vector<const char*> deviceExtension_{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
};