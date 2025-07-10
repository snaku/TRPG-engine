#include "Include/VulkanContext.hpp"
#include "Include/VulkanSwapchain.hpp"
#include "Include/Vertex.hpp"
#include "Include/UniformBufferObject.hpp"
#include "Include/VulkanMesh.hpp"
#include "Include/VulkanPipeline.hpp"

#include <fstream>
#include <array>
#include <iostream>

VulkanPipeline::VulkanPipeline(VulkanContext& ctx, const VulkanSwapchain& vkSwapchain,
                                const std::vector<std::unique_ptr<VulkanMesh>>& meshes) 
    : vkCtx_(ctx), vkSwapchain_(vkSwapchain), meshes_(meshes)
{
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
    if (vkCtx_.descriptorSetLayout != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorSetLayout(vkCtx_.device, vkCtx_.descriptorSetLayout, nullptr);
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

void VulkanPipeline::createShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
{
    vertexShaderCode_ = readShader(vertexPath);
    fragmentShaderCode_ = readShader(fragmentPath);
}

void VulkanPipeline::createPipeline()
{
    auto vertShaderCode = vertexShaderCode_;
    auto fragShaderCode = fragmentShaderCode_;

    VkShaderModule vertShaderModule = createShaderModule(vkCtx_.device, vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(vkCtx_.device, fragShaderCode);

    std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages 
    {
        createShaderStageInfo(VK_SHADER_STAGE_VERTEX_BIT, vertShaderModule),
        createShaderStageInfo(VK_SHADER_STAGE_FRAGMENT_BIT, fragShaderModule)
    };

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributesDescriptions();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)attributeDescriptions.size();
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkExtent2D extent = vkSwapchain_.getExtent();

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)extent.width;
    viewport.height = (float)extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = extent;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;

    rasterizer.cullMode = VK_CULL_MODE_NONE; // VK_CULL_MODE_BACK_BIT
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE; // VK_FRONT_FACE_CLOCKWISE
    
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    std::vector<VkDynamicState> dynamicStates = 
    {
        VK_DYNAMIC_STATE_LINE_WIDTH
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = (uint32_t)dynamicStates.size();
    dynamicState.pDynamicStates = dynamicStates.data();

    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    VK_CHECK(vkCreateDescriptorSetLayout(vkCtx_.device, &layoutInfo, nullptr, &vkCtx_.descriptorSetLayout));

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;  
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &vkCtx_.descriptorSetLayout; 

    VK_CHECK(vkCreatePipelineLayout(vkCtx_.device, &pipelineLayoutInfo, nullptr, &vkCtx_.pipelineLayout));

    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = 1;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = 1;

    VkDescriptorPool descriptorPool;
    VK_CHECK(vkCreateDescriptorPool(vkCtx_.device, &poolInfo, nullptr, &descriptorPool));

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &vkCtx_.descriptorSetLayout;

    VK_CHECK(vkAllocateDescriptorSets(vkCtx_.device, &allocInfo, &vkCtx_.descriptorSet));

    std::vector<VkWriteDescriptorSet> writes;

    for (const auto& mesh : meshes_)
    {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = mesh->getUniformBuffer();
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = vkCtx_.descriptorSet;
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;

        writes.push_back(descriptorWrite);
    }

    vkUpdateDescriptorSets(vkCtx_.device, (uint32_t)writes.size(), writes.data(), 0, nullptr);

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = vkCtx_.pipelineLayout;
    pipelineInfo.renderPass = vkCtx_.renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.pDynamicState = &dynamicState;

    VK_CHECK(vkCreateGraphicsPipelines(vkCtx_.device, VK_NULL_HANDLE,
                                    1, &pipelineInfo, nullptr,
                                    &vkCtx_.pipeline));

    // destroy shader module AFTER vkCreateGraphicsPipeline
    vkDestroyShaderModule(vkCtx_.device, fragShaderModule, nullptr);
    vkDestroyShaderModule(vkCtx_.device, vertShaderModule, nullptr);
}