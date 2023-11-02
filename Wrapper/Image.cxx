//
// Created by Mikołaj Wójcik on 02.11.2023.
//
module;

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <memory>

import LogicalDevice;
import PhysicalDevice;
import ImageInfo;
import MemoryHelper;

export module Image;
export class Image
{
    std::shared_ptr<LogicalDevice> pLogicalDevice;
    std::shared_ptr<PhysicalDevice> pPhysicalDevice;
public:
    VkImage image;
    ImageInfo imageInfo;

    Image(ImageInfo imageInfo, VkMemoryPropertyFlags properties,
                              VkDeviceMemory& imageMemory, std::shared_ptr<LogicalDevice> pLogicalDevice, std::shared_ptr<PhysicalDevice> pPhysicalDevice): imageInfo(imageInfo)
    {
        this->pLogicalDevice = pLogicalDevice;
        this->pPhysicalDevice = pPhysicalDevice;
        auto createInfo = this->imageInfo.GetCreateInfo();

        if (vkCreateImage(pLogicalDevice->device, &createInfo, nullptr, &image) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create image!");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(pLogicalDevice->device, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = MemoryHelper::findMemoryType(memRequirements.memoryTypeBits, properties, this->pPhysicalDevice);

        if (vkAllocateMemory(pLogicalDevice->device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate image memory!");
        }

        vkBindImageMemory(pLogicalDevice->device, image, imageMemory, 0);
    }

    ~Image()
    {
        vkDestroyImage(pLogicalDevice->device, image, nullptr);
    }
};