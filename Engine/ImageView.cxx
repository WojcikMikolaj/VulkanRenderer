//
// Created by Mikołaj Wójcik on 02.11.2023.
//
module;

#include <vulkan/vulkan.h>
#include <memory>
#include <stdexcept>

import LogicalDevice;
import Image;

export module ImageView;
export class ImageView
{
    std::shared_ptr<LogicalDevice> pLogicalDevice;
public:
    VkImageView imageView;
    ImageView(std::shared_ptr<LogicalDevice> pLogicalDevice, VkImage image, VkFormat format)
    {
        this->pLogicalDevice = pLogicalDevice;

        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(this->pLogicalDevice->device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create texture image view!");
        }
    }

    ~ImageView()
    {
        vkDestroyImageView(pLogicalDevice->device, imageView, nullptr);
    }
};