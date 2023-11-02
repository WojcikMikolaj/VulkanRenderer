//
// Created by mikow on 02.11.2023.
//
module;

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <memory>

import PhysicalDevice;

export module MemoryHelper;
export class MemoryHelper
{
public:
    static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, std::shared_ptr<PhysicalDevice>& pPhysicalDevice)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(pPhysicalDevice->physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }
};