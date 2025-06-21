#include "../Include/VulkanContext.hpp"
#include "../Include/VulkanPipeline.hpp"

#include <fstream>

VulkanPipeline::VulkanPipeline(VulkanContext& ctx) : vkCtx_(ctx)
{
    createPipeline();
}
VulkanPipeline::~VulkanPipeline()
{

}

[[nodiscard]] std::vector<char> VulkanPipeline::readShader(
    const std::filesystem::path& path)
{
    if (path.extension() != ".spv") 
        VK_THROW("Shader is not a SPIR-V file !");

    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) 
        VK_THROW("cannot open shader");

    std::size_t fileSize = file.tellg();

    file.seekg(0);

    std::vector<char> buffer(fileSize);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

VkShaderModule VulkanPipeline::createShaderModule(const std::vector<char>& code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    VK_CHECK(vkCreateShaderModule(vkCtx_.device, &createInfo, nullptr, &shaderModule));

    return shaderModule;
}

void VulkanPipeline::createPipeline()
{
    auto vertShaderCode = readShader("shaders/vert.spv");
}