#include "Include/VulkanTexture.hpp"

VulkanTexture::VulkanTexture(VulkanContext& ctx, const std::filesystem::path& path)
    : vkCtx_(ctx), texturePath_(path)
{

}
VulkanTexture::~VulkanTexture() noexcept
{

}

bool VulkanTexture::loadTexture()
{
    
}