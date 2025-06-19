#pragma once

struct VulkanContext;
class Window;

class VulkanInstance
{
public:
	VulkanInstance(VulkanContext& ctx, Window& window);
	~VulkanInstance() noexcept;

private:
	void createInstance();
	void createSurface();
	bool checkValidationLayer();

	VulkanContext& vkCtx_;
	Window& window_;
};