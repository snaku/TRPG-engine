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
	createInfo.enabledLayerCount = 0; // TODO

	VK_CHECK(vkCreateInstance(&createInfo, nullptr, &vkCtx_.instance));
}