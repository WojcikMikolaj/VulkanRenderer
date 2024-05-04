//
// Created by Mikołaj Wójcik on 05.11.2023.
//
module;

#include <memory>
#include <stdexcept>
#include <vulkan/vulkan.h>

import LogicalDevice;
import PhysicalDevice;
import Surface;
import QueueFamilyIndices;

export module CommandPool;
export class CommandPool
{
    std::shared_ptr<LogicalDevice> pLogicalDevice;
public:
    VkCommandPool commandPool;

    CommandPool(std::shared_ptr<LogicalDevice> pLogicalDevice, std::shared_ptr<PhysicalDevice>& pPhysicalDevice, std::shared_ptr<Surface>& pSurface)
    {
        this->pLogicalDevice = pLogicalDevice;

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = QueueFamilyIndices::findQueueFamilies(pPhysicalDevice->physicalDevice, pSurface->surface).graphicsFamily.value();

        if (VK_SUCCESS != vkCreateCommandPool(this->pLogicalDevice->device, &poolInfo, nullptr, &commandPool))
        {
            throw std::runtime_error("Failed to create command pool!");
        }
    }

    ~CommandPool()
    {
        vkDestroyCommandPool(pLogicalDevice->device, commandPool, nullptr);
        pLogicalDevice.reset();
    }
};