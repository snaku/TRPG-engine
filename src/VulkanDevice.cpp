#include "VulkanContext.hpp"
#include "VulkanDevice.hpp"

#include <iostream>

VulkanDevice::VulkanDevice(VulkanContext& ctx) : vkCtx_(ctx)
{
	pickPhysicalDevice();
}
VulkanDevice::~VulkanDevice() noexcept
{
	// vkDestroyDevice
}

void VulkanDevice::pickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(vkCtx_.instance, &deviceCount, nullptr);

	if (deviceCount == 0) 
		VK_THROW("no GPU supports vulkan !");

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(vkCtx_.instance, &deviceCount, devices.data());

	for (const auto& device : devices)
	{
		if (isDeviceSuitable(device))
		{
			vkCtx_.physicalDevice = device;
			break;
		}
	}

	if (vkCtx_.physicalDevice == VK_NULL_HANDLE)
		VK_THROW("vkCtx_.physicalDevice == VK_NULL_HANDLE");
}

bool VulkanDevice::isDeviceSuitable(VkPhysicalDevice device)
{
	VkPhysicalDeviceFeatures deviceFeatures;
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	
	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		deviceFeatures.geometryShader)
	{
		std::cout << "GPU: " << deviceProperties.deviceName << std::endl;
		return true;
	}
	
}
