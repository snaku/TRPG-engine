#pragma once

#include "vulkan/vulkan.h"

struct VulkanContext;
class VulkanDevice;

class VulkanSwapchain
{
public:
    VulkanSwapchain(VulkanContext& ctx, const VulkanDevice& vkDevice);
    ~VulkanSwapchain() noexcept;

private:
    struct SwapchainDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        VkSurfaceFormatKHR format;
        VkPresentModeKHR presentMode;
    };

    static VkPresentModeKHR pickPresentMode(VkPhysicalDevice device, VkSurfaceKHR surface);
    static VkSurfaceFormatKHR pickFormat(VkPhysicalDevice device, VkSurfaceKHR surface);
    SwapchainDetails chooseSwapchainSettings(VkPhysicalDevice device, VkSurfaceKHR surface);
    void createSwapchain(const VulkanDevice& vkDevice);

    VulkanContext& vkCtx_;
};