#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "Include/Vertex.hpp"
#include "Include/UniformBufferObject.hpp"

#include <vector>

struct VulkanContext;
class VulkanSwapchain;

class VulkanMesh
{
public:
    VulkanMesh(VulkanContext& ctx, const VulkanSwapchain& vkSwapchain,
               const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    ~VulkanMesh() noexcept;

    void bind(VkCommandBuffer cmd);
    void updateUniformBuffer(float deltaTime);

    // getters
    VkBuffer getVertexBuffer() const { return vertexBuffer_; }
    VkBuffer getUniformBuffer() const { return uniformBuffer_; }
    VkBuffer getIndexBuffer() const { return indexBuffer_; }
    size_t getVerticesSize() const { return vertices_.size(); }
    std::vector<uint32_t> getIndices() const { return indices_; }

private:
    void createBuffer(VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    
    template<typename T>
    static void copyMemory(VkDevice device, const T* src, VkDeviceMemory bufferMemory, VkDeviceSize bufferSize);

    std::vector<Vertex> vertices_;
    std::vector<uint32_t> indices_;

    VkBuffer vertexBuffer_ = VK_NULL_HANDLE;
    VkBuffer uniformBuffer_ = VK_NULL_HANDLE;
    VkBuffer indexBuffer_ = VK_NULL_HANDLE;
	VkDeviceMemory vertexBufferMemory_ = VK_NULL_HANDLE;
	VkDeviceMemory uniformBufferMemory_ = VK_NULL_HANDLE;
    VkDeviceMemory indexBufferMemory_ = VK_NULL_HANDLE;

    void* uniformBufferMap_ = nullptr;

    VulkanContext& vkCtx_;
    const VulkanSwapchain& vkSwapchain_;
};