#include "Include/VulkanContext.hpp"
#include "Include/VulkanSwapchain.hpp"
#include "Include/VulkanMesh.hpp"

#include <iostream>

VulkanMesh::VulkanMesh(VulkanContext& ctx, const VulkanSwapchain& vkSwapchain, const std::vector<Vertex>& vertices,const std::vector<uint32_t>& indices)
    : vkCtx_(ctx), vkSwapchain_(vkSwapchain), vertices_(std::move(vertices)),
    indices_(std::move(indices))
{
    VkDeviceSize vertexBufferSize = sizeof(vertices_[0]) * vertices_.size();
    VkDeviceSize uniformBufferSize = sizeof(UniformBufferObject);
    VkDeviceSize indexBufferSize = sizeof(indices_[0]) * indices_.size();

    // create vertex buffer
    createBuffer(vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertexBuffer_, vertexBufferMemory_);
    copyMemory(vkCtx_.device, vertices_.data(), vertexBufferMemory_, vertexBufferSize);

    // index buffer
    createBuffer(indexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, indexBuffer_, indexBufferMemory_);
    copyMemory(vkCtx_.device, indices_.data(), indexBufferMemory_, indexBufferSize);

    // create uniform buffer
    createBuffer(uniformBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, uniformBuffer_, uniformBufferMemory_);
    vkMapMemory(vkCtx_.device, uniformBufferMemory_, 0, uniformBufferSize, 0, &uniformBufferMap_);
}
VulkanMesh::~VulkanMesh() noexcept
{
    // index buffer
    vkDestroyBuffer(vkCtx_.device, indexBuffer_, nullptr);
    vkFreeMemory(vkCtx_.device, indexBufferMemory_, nullptr);
    
    // vertex buffer
    vkDestroyBuffer(vkCtx_.device, vertexBuffer_, nullptr);
    vkFreeMemory(vkCtx_.device, vertexBufferMemory_, nullptr);

    // uniform buffer
    vkDestroyBuffer(vkCtx_.device, uniformBuffer_, nullptr);
    vkUnmapMemory(vkCtx_.device, uniformBufferMemory_);
    vkFreeMemory(vkCtx_.device, uniformBufferMemory_, nullptr);
}

void VulkanMesh::createBuffer(VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
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

template<typename T>
void VulkanMesh::copyMemory(VkDevice device, const T* src, VkDeviceMemory bufferMemory, VkDeviceSize bufferSize)
{
    void* data = nullptr;
    vkMapMemory(device, bufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, src, bufferSize);
    vkUnmapMemory(device, bufferMemory);
}

void VulkanMesh::bind(VkCommandBuffer cmd)
{
    VkDeviceSize offsets[] = { 0 };

    vkCmdBindVertexBuffers(cmd, 0, 1, &vertexBuffer_, offsets);
    vkCmdBindIndexBuffer(cmd, indexBuffer_, 0, VK_INDEX_TYPE_UINT32);
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

    // glm::vec3(2.0f, 2.0f, 1.0f)
    ubo.view = glm::lookAt(
        glm::vec3(camX, camY, 1.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    ubo.proj = glm::perspective(
        glm::radians(45.0f),
        vkSwapchain_.getExtent().width / (float)vkSwapchain_.getExtent().height,
        0.1f,
        100.0f
    );
    ubo.proj[1][1] *= -1;

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

    memcpy(uniformBufferMap_, &ubo, sizeof(ubo));
}