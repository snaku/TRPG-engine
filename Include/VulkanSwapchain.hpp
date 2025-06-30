#pragma once

#include "vulkan/vulkan.h"

struct VulkanContext;
class Window;
class VulkanDevice;

class VulkanSwapchain
{
public:
    VulkanSwapchain(VulkanContext& ctx, const VulkanDevice& vkDevice, const Window& window);
    ~VulkanSwapchain() noexcept;

    // getters
    VkFormat getFormat() const { return format_; }
    VkExtent2D getExtent() const { return extent_; }

private:
    struct SwapchainDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        VkExtent2D extent;
        VkSurfaceFormatKHR format;
        VkPresentModeKHR presentMode;
        uint32_t minImageCount;
    };

    static VkPresentModeKHR pickPresentMode(VkPhysicalDevice device, VkSurfaceKHR surface);
    static VkSurfaceFormatKHR pickFormat(VkPhysicalDevice device, VkSurfaceKHR surface);
    static VkExtent2D pickSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilites, uint32_t width, uint32_t height);
    
    SwapchainDetails chooseSwapchainSettings(VkPhysicalDevice device, VkSurfaceKHR surface, uint32_t width, uint32_t height);
    
    void createSwapchain(const VulkanDevice& vkDevice, const Window& window);
    void createImageViews(VkFormat format);

    VulkanContext& vkCtx_;
    VkFormat format_;
    VkExtent2D extent_;
};