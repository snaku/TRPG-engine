#pragma once

#include <optional>

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
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	void createLogicalDevice();
	void createCommandPool();

	VulkanContext& vkCtx_;
};