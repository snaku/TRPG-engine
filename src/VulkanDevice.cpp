#include "Include/VulkanContext.hpp"
#include "Include/VulkanDevice.hpp"

#include <iostream>
#include <set>
#include <algorithm>

VulkanDevice::VulkanDevice(VulkanContext& ctx) : vkCtx_(ctx)
{
	pickPhysicalDevice();
	indices_ = findQueueFamilies(vkCtx_.physicalDevice);
	createLogicalDevice();
}
VulkanDevice::~VulkanDevice() noexcept
{
	if (vkCtx_.device != VK_NULL_HANDLE)
	{
		vkDestroyDevice(vkCtx_.device, nullptr);
	}
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

bool VulkanDevice::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	
	if (extensionCount == 0)
		VK_THROW("Device extension count is 0");

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::vector<std::string> requiredExtensions(deviceExtension_.begin(), deviceExtension_.end());

	auto it = std::remove_if(requiredExtensions.begin(), requiredExtensions.end(),
	[&](const std::string& ext)
	{
		for (const auto& extension : availableExtensions)
		{
			if (ext == extension.extensionName)
				return true;
		}

		return false;
	});

	requiredExtensions.erase(it, requiredExtensions.end());

    return requiredExtensions.empty();
}
bool VulkanDevice::isDeviceSuitable(VkPhysicalDevice device)
{
	VkPhysicalDeviceFeatures deviceFeatures;
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		deviceFeatures.geometryShader && extensionsSupported)
	{
		std::cout << "GPU: " << deviceProperties.deviceName << std::endl;
		return true;
	}
	
	return false;
}

VulkanDevice::QueueFamilyIndices VulkanDevice::findQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	uint32_t queueCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = VK_FALSE;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vkCtx_.surface, &presentSupport);

		if (presentSupport)
		{
			indices.presentFamily = i;
		}

		if (indices.isComplete()) break;

		i++;
	}

	return indices;
}

void VulkanDevice::createLogicalDevice()
{
	std::cout << "gfx queue index: " << indices_.graphicsFamily.value() <<
		"\npresent queue index: " << indices_.presentFamily.value() << std::endl;

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueFamilies = { indices_.graphicsFamily.value(), indices_.presentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.enabledExtensionCount = (uint32_t)deviceExtension_.size();
	createInfo.ppEnabledExtensionNames = deviceExtension_.data();

	VK_CHECK(vkCreateDevice(vkCtx_.physicalDevice, &createInfo, nullptr, &vkCtx_.device));

	vkGetDeviceQueue(vkCtx_.device, indices_.graphicsFamily.value(), 0, &vkCtx_.graphicsQueue);
	vkGetDeviceQueue(vkCtx_.device, indices_.presentFamily.value(), 0, &vkCtx_.presentQueue);
}