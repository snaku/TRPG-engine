#include "Include/VulkanContext.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "Include/VulkanTexture.hpp"

#include <iostream>

VulkanTexture::VulkanTexture(VulkanContext& ctx) : vkCtx_(ctx)
{
}
VulkanTexture::~VulkanTexture() noexcept
{
    vkDestroyBuffer(vkCtx_.device, stagingBuffer_, nullptr);
    vkFreeMemory(vkCtx_.device, stagingBufferMemory_, nullptr);
}

std::pair<stbi_uc*, VkDeviceSize> VulkanTexture::loadTexture(const std::filesystem::path& texturePath)
{
    int width, height, channels;
    stbi_uc* texture = stbi_load(texturePath.u8string().c_str(), &width, &height, &channels, STBI_rgb_alpha);
    
    if (!texture || width <= 0 || height <= 0)
        VK_THROW("Failed to load " + texturePath.filename().string());
    
    const VkDeviceSize imgSize = width * height * 4;

    std::cout << "Texture loaded: " << texturePath << ". Texture width: " << width << " Texture height: " << height << std::endl;

    return {texture, imgSize};
}

void VulkanTexture::createTextureImage(const std::filesystem::path& texturePath)
{
    auto [texture, imgSize] = loadTexture(texturePath);

    createBuffer(imgSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, stagingBuffer_, stagingBufferMemory_);

    void* data;
    vkMapMemory(vkCtx_.device, stagingBufferMemory_, 0, imgSize, 0, &data);
    memcpy(data, texture, (std::size_t)imgSize);
    vkUnmapMemory(vkCtx_.device, stagingBufferMemory_);

    stbi_image_free(texture);
}

// temporary
void VulkanTexture::createBuffer(VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.usage = usage;
    bufferInfo.size = bufferSize;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VK_CHECK(vkCreateBuffer(vkCtx_.device, &bufferInfo, nullptr, &buffer));

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vkCtx_.device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    VK_CHECK(vkAllocateMemory(vkCtx_.device, &allocInfo, nullptr, &bufferMemory));

    vkBindBufferMemory(vkCtx_.device, buffer, bufferMemory, 0);
}

uint32_t VulkanTexture::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(vkCtx_.physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
        {
            return i;
        }
    }

    VK_THROW("Failed to find suitable memory type");
}