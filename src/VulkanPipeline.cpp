#include "../Include/VulkanContext.hpp"
#include "../Include/VulkanPipeline.hpp"

#include <fstream>
#include <array>

VulkanPipeline::VulkanPipeline(VulkanContext& ctx) : vkCtx_(ctx)
{
    createPipeline();
}
VulkanPipeline::~VulkanPipeline() noexcept
{
    if (vkCtx_.pipeline != VK_NULL_HANDLE)
    {
        vkDestroyPipeline(vkCtx_.device, vkCtx_.pipeline, nullptr);
    }
    if (vkCtx_.pipelineLayout != VK_NULL_HANDLE)
    {
        vkDestroyPipelineLayout(vkCtx_.device, vkCtx_.pipelineLayout, nullptr);
    }
}

[[nodiscard]] std::vector<char> VulkanPipeline::readShader(
    const std::filesystem::path& path)
{
    if (path.extension() != ".spv") 
        VK_THROW("Shader is not a SPIR-V file !");

    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) 
        VK_THROW("cannot open shader at " + path.string());

    std::size_t fileSize = file.tellg();

    file.seekg(0);

    std::vector<char> buffer(fileSize);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

VkShaderModule VulkanPipeline::createShaderModule(VkDevice device, const std::vector<char>& code)
{
    // SPIR-V code must be 4 bytes aligned or else it will crash
    if (code.size() % 4 != 0)
        VK_THROW("Shader code is not a multiple of 4 !");

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    VK_CHECK(vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule));

    return shaderModule;
}

VkPipelineShaderStageCreateInfo VulkanPipeline::createShaderStageInfo(
    VkShaderStageFlagBits stage, VkShaderModule module)
{
    VkPipelineShaderStageCreateInfo shaderStageInfo{};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = stage;
    shaderStageInfo.module = module;
    shaderStageInfo.pName = "main";

    return shaderStageInfo;
}

void VulkanPipeline::createPipeline()
{
    auto vertShaderCode = readShader("shaders/vert.spv");
    auto fragShaderCode = readShader("shaders/frag.spv");

    VkShaderModule vertShaderModule = createShaderModule(vkCtx_.device, vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(vkCtx_.device, fragShaderCode);

    std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages 
    {
        createShaderStageInfo(VK_SHADER_STAGE_VERTEX_BIT, vertShaderModule),
        createShaderStageInfo(VK_SHADER_STAGE_FRAGMENT_BIT, fragShaderModule)
    };

    // VK_CHECK(vkCreateGraphicsPipelines(vkCtx_.device, ));

    // destroy shader module AFTER vkCreateGraphicsPipeline
    vkDestroyShaderModule(vkCtx_.device, fragShaderModule, nullptr);
    vkDestroyShaderModule(vkCtx_.device, vertShaderModule, nullptr);
}