#pragma once

#include <vulkan/vulkan.h>
#include <filesystem>

struct VulkanContext;

class VulkanTexture
{
public:
    VulkanTexture(VulkanContext& ctx, const std::filesystem::path& path);
    ~VulkanTexture() noexcept;

    bool loadTexture();
    
private:
    VulkanContext& vkCtx_;
    const std::filesystem::path texturePath_;
    VkImage image_;
    VkImageView imageView_;
    VkDeviceMemory imageMemory_;
    VkSampler sampler_;
};