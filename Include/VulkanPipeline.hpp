#pragma once

#include <vulkan/vulkan.h>

#include <vector>
#include <filesystem>

struct VulkanContext;

class VulkanPipeline
{
public:
    explicit VulkanPipeline(VulkanContext& ctx);
    ~VulkanPipeline() noexcept;

private:
    [[nodiscard]] std::vector<char> readShader(const std::filesystem::path& path);
    
    static VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);
    static VkPipelineShaderStageCreateInfo createShaderStageInfo(VkShaderStageFlagBits stage, VkShaderModule module);

    void createPipeline();

    VulkanContext& vkCtx_;
};