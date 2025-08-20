#define STB_IMAGE_IMPLEMENTATION

#include <Renderer/VulkanRenderer/VulkanContext.hpp>
#include <Renderer/VulkanRenderer/VulkanTexture.hpp>

// std
#include <iostream>

VulkanTexture::VulkanTexture(VulkanContext& ctx) : vkCtx_(ctx)
{
}
VulkanTexture::~VulkanTexture() noexcept
{
    vkDestroyBuffer(vkCtx_.device, stagingBuffer_, nullptr);
    vkFreeMemory(vkCtx_.device, stagingBufferMemory_, nullptr);
}

VulkanTexture::TextureDetails VulkanTexture::loadTexture(const std::filesystem::path& texturePath)
{
    int width, height, channels;
    stbi_uc* texture = stbi_load(texturePath.u8string().c_str(), &width, &height, &channels, STBI_rgb_alpha);
    
    if (!texture || width <= 0 || height <= 0)
        VK_THROW("Failed to load " + texturePath.filename().string());
    
    const VkDeviceSize imgSize = width * height * 4;

    std::cout << "Texture loaded: " << texturePath << ". Texture width: " << width << " Texture height: " << height << std::endl;

    TextureDetails details{};
    details.texture = texture;
    details.width = width;
    details.height = height;
    details.channels = channels;
    details.imgSize = imgSize;

    return details;
}

void VulkanTexture::createTextureImage(const std::filesystem::path& texturePath)
{
    TextureDetails details = loadTexture(texturePath);

    createBuffer(details.imgSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, stagingBuffer_, stagingBufferMemory_);

    void* data;
    vkMapMemory(vkCtx_.device, stagingBufferMemory_, 0, details.imgSize, 0, &data);
    memcpy(data, details.texture, (std::size_t)details.imgSize);
    vkUnmapMemory(vkCtx_.device, stagingBufferMemory_);

    stbi_image_free(details.texture);

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = (uint32_t)details.width;
    imageInfo.extent.height = (uint32_t)details.height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

    VK_CHECK(vkCreateImage(vkCtx_.device, &imageInfo, nullptr, &image_));

    VkMemoryRequirements memoryRequirements{};
    vkGetImageMemoryRequirements(vkCtx_.device, image_, &memoryRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memoryRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    VK_CHECK(vkAllocateMemory(vkCtx_.device, &allocInfo, nullptr, &imageMemory_));

    vkBindImageMemory(vkCtx_.device, image_, imageMemory_, 0);
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