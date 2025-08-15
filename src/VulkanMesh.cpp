#include "Include/VulkanContext.hpp"
#include "Include/VulkanSwapchain.hpp"
#include "Include/Camera.hpp"
#include "Include/VulkanMesh.hpp"

#include <iostream>

VulkanMesh::VulkanMesh(VulkanContext& ctx, const VulkanSwapchain& vkSwapchain, std::vector<Vertex> vertices, std::vector<uint32_t> indices)
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

void VulkanMesh::sendPushConstantData(VkCommandBuffer cmd, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size)
{
    vkCmdPushConstants(cmd, layout, stageFlags, offset, size, &pushConstantModel_);
}

void VulkanMesh::updatePushConstantData(const glm::mat4& model)
{
    pushConstantModel_ = model;
}

void VulkanMesh::updateUniformBuffer(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
    UniformBufferObject ubo{};

    ubo.view = viewMatrix;
    ubo.proj = projectionMatrix;

    memcpy(uniformBufferMap_, &ubo, sizeof(ubo));
}