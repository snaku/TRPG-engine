#pragma once

#include <vulkan/vulkan.h>

// std
#include <vector>
#include <filesystem>
#include <memory>

struct VulkanContext;
class VulkanSwapchain;
class VulkanMesh;

class VulkanPipeline
{
public:
    VulkanPipeline(VulkanContext& ctx, const VulkanSwapchain& vkSwapchain, const std::vector<std::unique_ptr<VulkanMesh>>& meshes);
    ~VulkanPipeline() noexcept;

    void createShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
    void createPipeline();
private:
    [[nodiscard]] std::vector<char> readShader(const std::filesystem::path& path);
    
    static VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);
    static VkPipelineShaderStageCreateInfo createShaderStageInfo(VkShaderStageFlagBits stage, VkShaderModule module);

    VulkanContext& vkCtx_;
    const VulkanSwapchain& vkSwapchain_;
    const std::vector<std::unique_ptr<VulkanMesh>>& meshes_;
    
    std::vector<char> vertexShaderCode_;
    std::vector<char> fragmentShaderCode_;
};