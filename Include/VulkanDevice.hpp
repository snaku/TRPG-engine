#pragma once

struct VulkanContext;

class VulkanDevice
{
public:
	explicit VulkanDevice(VulkanContext& ctx);
	~VulkanDevice() noexcept;

private:
	void pickPhysicalDevice();
	VulkanContext& vkCtx_;
};