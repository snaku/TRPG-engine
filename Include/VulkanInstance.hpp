#pragma once

struct VulkanContext;

class VulkanInstance
{
public:
	explicit VulkanInstance(VulkanContext& ctx);
	~VulkanInstance() noexcept;

private:
	void createInstance();

	VulkanContext& vkCtx_;
};