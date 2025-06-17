#include "VulkanContext.hpp"
#include "VulkanDevice.hpp"

VulkanDevice::VulkanDevice(VulkanContext& ctx) : vkCtx_(ctx)
{
	pickPhysicalDevice();
}
VulkanDevice::~VulkanDevice() noexcept
{

}

void VulkanDevice::pickPhysicalDevice()
{

}