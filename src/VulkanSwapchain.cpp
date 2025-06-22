#include "../Include/VulkanContext.hpp"
#include "../Include/VulkanSwapchain.hpp"
#include "../Include/VulkanDevice.hpp"

#include <array>

VulkanSwapchain::VulkanSwapchain(VulkanContext& ctx, const VulkanDevice& vkDevice) : vkCtx_(ctx)
{
    createSwapchain(vkDevice);
}
VulkanSwapchain::~VulkanSwapchain() noexcept
{
    if (vkCtx_.swapchain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(vkCtx_.device, vkCtx_.swapchain, nullptr);
        vkCtx_.swapchain = VK_NULL_HANDLE;
    }
}

VkPresentModeKHR VulkanSwapchain::pickPresentMode(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    uint32_t presentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
    
    if (presentModeCount == 0)
    {
        VK_THROW("presentModeCount is 0 !");
    }
    
    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, presentModes.data());

    for (const auto& mode : presentModes)
    {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return VK_PRESENT_MODE_MAILBOX_KHR;
        }
    }

    // if we can't have VK_PRESENT_MODE_MAILBOX_KHR
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkSurfaceFormatKHR VulkanSwapchain::pickFormat(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    
    if (formatCount == 0)
        VK_THROW("formatCount is 0 !");
    
    std::vector<VkSurfaceFormatKHR> availableFormats(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, availableFormats.data());

    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB 
            && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    // if we can't have VK_FORMAT_B8G8R8A8_SRGB and VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
    return availableFormats[0];
}

VulkanSwapchain::SwapchainDetails VulkanSwapchain::chooseSwapchainSettings(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    SwapchainDetails details;
    details.presentMode = pickPresentMode(device, surface);
    details.format = pickFormat(device, surface);

    return details;
}

void VulkanSwapchain::createSwapchain(const VulkanDevice& vkDevice)
{
    SwapchainDetails details = chooseSwapchainSettings(vkCtx_.physicalDevice, vkCtx_.surface);

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = vkCtx_.surface;
    createInfo.imageFormat = details.format.format;
    createInfo.imageColorSpace = details.format.colorSpace;
    createInfo.presentMode = details.presentMode;
    createInfo.imageExtent = details.capabilities.currentExtent;

    std::array<uint32_t, 2> queueFamilyIndices{vkDevice.getGraphicsFamilyIndices(), vkDevice.getPresentFamilyIndices()};

    if (queueFamilyIndices[0] != queueFamilyIndices[1])
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }
}