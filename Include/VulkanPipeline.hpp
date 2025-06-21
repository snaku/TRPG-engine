#pragma once

#include "vulkan/vulkan.h"

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
    VkShaderModule createShaderModule(const std::vector<char>& code);
    void createPipeline();

    VulkanContext& vkCtx_;
};