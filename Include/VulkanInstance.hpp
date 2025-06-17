#pragma once

struct VulkanContext;

class VulkanInstance
{
public:
	explicit VulkanInstance(VulkanContext& ctx);
	~VulkanInstance() noexcept;

private:
	void createInstance();
	bool checkValidationLayer();

	VulkanContext& vkCtx_;
};