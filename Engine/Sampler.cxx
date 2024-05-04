//
// Created by Mikołaj Wójcik on 11.11.2023.
//
module;

#include <vulkan/vulkan.h>
#include <memory>
#include <stdexcept>

import PhysicalDevice;
import LogicalDevice;

export module Sampler;
export class Sampler
{
    std::shared_ptr<LogicalDevice> pLogicalDevice;

public:
    VkSampler sampler;

    Sampler(std::shared_ptr<PhysicalDevice>& pPhysicalDevice, std::shared_ptr<LogicalDevice> pLogicalDevice)
    {
        this->pLogicalDevice = pLogicalDevice;

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;

        auto properties = pPhysicalDevice->GetProperties();

        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        if (vkCreateSampler(this->pLogicalDevice->device, &samplerInfo, nullptr, &sampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }
    }

    ~Sampler()
    {
        vkDestroySampler(pLogicalDevice->device, sampler, nullptr);
    }
};