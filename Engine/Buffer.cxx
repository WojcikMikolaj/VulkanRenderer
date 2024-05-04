//
// Created by Mikołaj Wójcik on 02.11.2023.
//
module;

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <memory>

import LogicalDevice;
import PhysicalDevice;
import MemoryHelper;

export module Buffer;
export class Buffer
{
    VkDeviceMemory bufferMemory;
    std::shared_ptr<LogicalDevice> pLogicalDevice;
    std::shared_ptr<PhysicalDevice> pPhysicalDevice;

public:
    VkBuffer buffer;
    Buffer(VkDeviceSize size,
                               VkBufferUsageFlags usage,
                               VkMemoryPropertyFlags properties,
                               std::shared_ptr<LogicalDevice> pLogicalDevice,
                               std::shared_ptr<PhysicalDevice> pPhysicalDevice)
    {
        this->pLogicalDevice = pLogicalDevice;
        this->pPhysicalDevice = pPhysicalDevice;

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(pLogicalDevice->device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(pLogicalDevice->device, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = MemoryHelper::findMemoryType(memRequirements.memoryTypeBits, properties, pPhysicalDevice);

        if (vkAllocateMemory(this->pLogicalDevice->device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate buffer memory!");
        }

        vkBindBufferMemory(this->pLogicalDevice->device, buffer, bufferMemory, 0);
    }

    VkResult MapMemory(VkDeviceSize size, VkMemoryMapFlags** flags, void** ppData)
    {
        return vkMapMemory(pLogicalDevice->device, bufferMemory, 0, size, 0, ppData);
    }

    void UnmapMemory()
    {
        vkUnmapMemory(pLogicalDevice->device, bufferMemory);
    }

    ~Buffer()
    {
        vkDestroyBuffer(pLogicalDevice->device, buffer, nullptr);
        vkFreeMemory(pLogicalDevice->device, bufferMemory, nullptr);
    }
};
