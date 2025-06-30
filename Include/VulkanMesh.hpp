#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "../Include/Vertex.hpp"
#include "../Include/UniformBufferObject.hpp"

#include <vector>

struct VulkanContext;
class VulkanSwapchain;
class VulkanMesh
{
public:
    VulkanMesh(VulkanContext& ctx, const VulkanSwapchain& vkSwapchain,
               std::vector<Vertex> vertices);
    ~VulkanMesh() noexcept;

    void createVertexBuffer();
    void createbuffer();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void updateUniformBuffer(float deltaTime);

    VkBuffer getVertexBuffer() const { return vertexBuffer_; }
    VkBuffer getUniformBuffer() const { return uniformBuffer_; }
    size_t getVerticesSize() const { return vertices_.size(); }
private:
    /*const std::vector<Vertex> vertices_ = 
    {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };*/
    std::vector<Vertex> vertices_;
    VkBuffer vertexBuffer_ = VK_NULL_HANDLE;
    VkBuffer uniformBuffer_ = VK_NULL_HANDLE;
	VkDeviceMemory vertexBufferMemory_ = VK_NULL_HANDLE;
	VkDeviceMemory uniformBufferMemory_ = VK_NULL_HANDLE;

    VulkanContext& vkCtx_;
    const VulkanSwapchain& vkSwapchain_;
};