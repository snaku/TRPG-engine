#include "../Include/VulkanContext.hpp"
#include "../Include/VulkanSwapchain.hpp"
#include "../Include/VulkanMesh.hpp"

#include <iostream>

VulkanMesh::VulkanMesh(VulkanContext& ctx, const VulkanSwapchain& vkSwapchain, std::vector<Vertex> vertices) 
    : vkCtx_(ctx), vkSwapchain_(vkSwapchain), vertices_(std::move(vertices))
{
    createVertexBuffer();
    createbuffer();
}
VulkanMesh::~VulkanMesh() noexcept
{
    // vertex buffer
    vkDestroyBuffer(vkCtx_.device, vertexBuffer_, nullptr);
    vkFreeMemory(vkCtx_.device, vertexBufferMemory_, nullptr);

    // uniform buffer
    vkDestroyBuffer(vkCtx_.device, uniformBuffer_, nullptr);
    vkFreeMemory(vkCtx_.device, uniformBufferMemory_, nullptr);
}

void VulkanMesh::createVertexBuffer()
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(vertices_[0]) * vertices_.size();
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VK_CHECK(vkCreateBuffer(vkCtx_.device, &bufferInfo, nullptr, &vertexBuffer_));

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vkCtx_.device, vertexBuffer_, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    VK_CHECK(vkAllocateMemory(vkCtx_.device, &allocInfo, nullptr, &vertexBufferMemory_));

    vkBindBufferMemory(vkCtx_.device, vertexBuffer_, vertexBufferMemory_, 0);

    void* data;
    vkMapMemory(vkCtx_.device, vertexBufferMemory_, 0, bufferInfo.size, 0, &data);
    std::cout << "vertices size: " << vertices_.size() << std::endl;
    memcpy(data, vertices_.data(), (size_t)bufferInfo.size);
    vkUnmapMemory(vkCtx_.device, vertexBufferMemory_);
}

void VulkanMesh::createbuffer()
{
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = bufferSize;
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VK_CHECK(vkCreateBuffer(vkCtx_.device, &bufferInfo, nullptr, &uniformBuffer_));

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vkCtx_.device, uniformBuffer_, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(
        memRequirements.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    VK_CHECK(vkAllocateMemory(vkCtx_.device, &allocInfo, nullptr, &uniformBufferMemory_));
    vkBindBufferMemory(vkCtx_.device, uniformBuffer_, uniformBufferMemory_, 0);
}

uint32_t VulkanMesh::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
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

void VulkanMesh::updateUniformBuffer(float deltaTime)
{
    UniformBufferObject ubo{};
    // ubo.model = glm::rotate(glm::mat4(1.0f), time, glm::vec3(0.0f, 0.0f, 1.0f));

    static float x = 0.0f;
    static float y = 0.0f;
    static float z = 2.0f;

    static bool caca = true;

    /*if (caca)
    {
        x += 1.0f * deltaTime;
        if (x >= 3.0f) 
        {
            x = 3.0f;
            caca = false;
        }
    }
    else
    {
        x -= 1.0f * deltaTime;
        if (x <= 0.0f) 
        {
            x = 0.0f;
            caca = true;
        }
    }*/

    

    float radius = 2.0f;
    static float angle = 0.0f;
    angle += deltaTime * 1.0f;

    float camX = radius * cos(angle);
    float camY = radius * sin(angle);

    ubo.model = glm::mat4(1.0f);

    ubo.view = glm::lookAt(
        glm::vec3(camX, camY, 8.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    ubo.proj = glm::perspective(
        glm::radians(45.0f),
        vkSwapchain_.getExtent().width / (float)vkSwapchain_.getExtent().height,
        0.1f,
        10.f
    );
    // ubo.proj[1][1] *= -1;

    /*static float scale = 0.25f;
    static bool isIncreasing = true;
    if (isIncreasing)
    {
        scale += deltaTime * 0.50f;
        if (scale >= 1.5f) isIncreasing = false;
    }
    else 
    {
        scale -= deltaTime * 0.50f;
        if (scale <= 0.1f) isIncreasing = true;
    }

    ubo.model = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));*/

    void* data;
    vkMapMemory(vkCtx_.device, uniformBufferMemory_, 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(vkCtx_.device, uniformBufferMemory_);
}