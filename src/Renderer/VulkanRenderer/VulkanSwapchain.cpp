#include <Core/Window.hpp>
#include <Renderer/VulkanRenderer/VulkanContext.hpp>
#include <Renderer/VulkanRenderer/VulkanDevice.hpp>
#include <Renderer/VulkanRenderer/VulkanSwapchain.hpp>

// std
#include <array>
#include <algorithm>

VulkanSwapchain::VulkanSwapchain(VulkanContext& ctx, const VulkanDevice& vkDevice, const Window& window) : vkCtx_(ctx)
{
    createSwapchain(vkDevice, window);
}
VulkanSwapchain::~VulkanSwapchain() noexcept
{
    for (const auto& imgView : vkCtx_.swapchainImageViews)
    {
        vkDestroyImageView(vkCtx_.device, imgView, nullptr);
    }
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

    // GPU is at almost 100% when using VK_PRESENT_MODE_MAILBOX_KHR 
    /*for (const auto& mode : presentModes)
    {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return VK_PRESENT_MODE_FIFO_KHR;
        }
    }*/

    // if we can't have VK_PRESENT_MODE_MAILBOX_KHR (also it's always available)
    return VK_PRESENT_MODE_IMMEDIATE_KHR;
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

VkExtent2D VulkanSwapchain::pickSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilites,
                                                uint32_t width,
                                                uint32_t height)
{
    if (capabilites.currentExtent.width != UINT32_MAX)
        return capabilites.currentExtent;

    VkExtent2D actual { width, height };
    actual.width = std::clamp(actual.width,
                            capabilites.minImageExtent.width,
                            capabilites.maxImageExtent.width);

    actual.height = std::clamp(actual.height,
                            capabilites.minImageExtent.height,
                            capabilites.maxImageExtent.height);

    return actual;
}

VulkanSwapchain::SwapchainDetails VulkanSwapchain::chooseSwapchainSettings(VkPhysicalDevice device, VkSurfaceKHR surface, uint32_t width, uint32_t height)
{
    SwapchainDetails details;
    details.presentMode = pickPresentMode(device, surface);
    details.format = pickFormat(device, surface);

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    details.extent = pickSwapchainExtent(details.capabilities, width, height);

    details.minImageCount = details.capabilities.minImageCount + 1;

    if (details.capabilities.maxImageCount > 0 && details.minImageCount > details.capabilities.maxImageCount)
        details.minImageCount = details.capabilities.maxImageCount;

    format_ = details.format.format;
    extent_ = details.extent;

    return details;
}

// calls createImageViews
void VulkanSwapchain::createSwapchain(const VulkanDevice& vkDevice, const Window& window)
{
    uint32_t height = window.getHeight();
    uint32_t width = window.getWidth();
    SwapchainDetails details = chooseSwapchainSettings(vkCtx_.physicalDevice, vkCtx_.surface, width, height);

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = vkCtx_.surface;
    createInfo.imageFormat = details.format.format;
    createInfo.imageColorSpace = details.format.colorSpace;
    createInfo.presentMode = details.presentMode;
    createInfo.imageExtent = details.extent;
    createInfo.minImageCount = details.minImageCount;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.preTransform = details.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.clipped = VK_TRUE;
    
    // TODO: handle window resize
    createInfo.oldSwapchain = VK_NULL_HANDLE;

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

    VK_CHECK(vkCreateSwapchainKHR(vkCtx_.device, &createInfo, nullptr, &vkCtx_.swapchain));

    vkGetSwapchainImagesKHR(vkCtx_.device, vkCtx_.swapchain, &details.minImageCount, nullptr);
    vkCtx_.swapchainImages.resize(details.minImageCount);
    vkGetSwapchainImagesKHR(vkCtx_.device, vkCtx_.swapchain, &details.minImageCount, vkCtx_.swapchainImages.data());

    createImageViews(details.format.format);
}

void VulkanSwapchain::createImageViews(VkFormat format)
{
    vkCtx_.swapchainImageViews.resize(vkCtx_.swapchainImages.size());
    
    for (std::size_t i = 0; i < vkCtx_.swapchainImages.size(); i++)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = vkCtx_.swapchainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = format;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        VK_CHECK(vkCreateImageView(vkCtx_.device, &createInfo, nullptr, &vkCtx_.swapchainImageViews[i]));
    }
}