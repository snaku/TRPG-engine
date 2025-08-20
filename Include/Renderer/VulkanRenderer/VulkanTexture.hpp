#pragma once

#include <vulkan/vulkan.h>
#include <stb_image.h>

// std
#include <filesystem>

struct VulkanContext;

class VulkanTexture
{
public:
    explicit VulkanTexture(VulkanContext& ctx);
    ~VulkanTexture() noexcept;

    void createTextureImage(const std::filesystem::path& texturePath);

    struct TextureDetails
    {
        stbi_uc* texture;
        int width;
        int height; 
        int channels;
        VkDeviceSize imgSize;
    };
private:
    TextureDetails loadTexture(const std::filesystem::path& texturePath);

    // createBuffer and findMemoryType are temporary here
    void createBuffer(VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    VulkanContext& vkCtx_;

    VkBuffer stagingBuffer_;
    VkDeviceMemory stagingBufferMemory_;
    VkImage image_;
    VkImageView imageView_;
    VkDeviceMemory imageMemory_;
    VkSampler sampler_;
};