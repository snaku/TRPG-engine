#include <iostream>
#include <GLFW/glfw3.h>
#include "VulkanContext.hpp"
#include "VulkanInstance.hpp"

VulkanInstance::VulkanInstance(VulkanContext& ctx) : vkCtx_(ctx)
{
	createInstance();
}
VulkanInstance::~VulkanInstance() noexcept
{
	if (vkCtx_.instance != VK_NULL_HANDLE)
	{
		vkDestroyInstance(vkCtx_.instance, nullptr);
	}
}

void VulkanInstance::createInstance()
{
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "TRPG Engine";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_3;

	uint32_t glfwExtCount = 0;
	const char** glfwExt;

	glfwExt = glfwGetRequiredInstanceExtensions(&glfwExtCount);

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = glfwExtCount;
	createInfo.ppEnabledExtensionNames = glfwExt;

#ifdef _DEBUG
	vkCtx_.enableValidationLayers = true;
	vkCtx_.validationLayers.push_back("VK_LAYER_KHRONOS_validation");
#else
	vkCtx_.enableValidationLayers = false;
#endif

	if (vkCtx_.enableValidationLayers && checkValidationLayer())
	{
		createInfo.enabledLayerCount = (uint32_t)vkCtx_.validationLayers.size();
		createInfo.ppEnabledLayerNames = vkCtx_.validationLayers.data();
		std::cout << "validation layer !" << std::endl;
	}
	else
	{
		createInfo.enabledLayerCount = 0;
		createInfo.ppEnabledLayerNames = nullptr;
	}

	VK_CHECK(vkCreateInstance(&createInfo, nullptr, &vkCtx_.instance));
}

bool VulkanInstance::checkValidationLayer()
{
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> layerProperties(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, layerProperties.data());

	for (const auto& lp : layerProperties)
	{
		if (strcmp(lp.layerName, "VK_LAYER_KHRONOS_validation") == 0)
		{
			return true;
		}
	}

	return false;
}
