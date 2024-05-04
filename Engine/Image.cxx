//
// Created by Mikołaj Wójcik on 02.11.2023.
//
module;

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <memory>
#include <string>
#include "../stb/stb_image.h"

import LogicalDevice;
import PhysicalDevice;
import ImageInfo;
import MemoryHelper;
import Buffer;
import CommandPool;
import SingleTimeCommandBuffer;

export module Image;
export class Image
{
    std::shared_ptr<LogicalDevice> pLogicalDevice;
    std::shared_ptr<PhysicalDevice> pPhysicalDevice;
    std::shared_ptr<CommandPool> pCommandPool;

    void CreateImage(VkMemoryPropertyFlags properties)
    {
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
public:
    VkImage image;
    ImageInfo imageInfo;
    VkDeviceMemory imageMemory;



    Image(std::string pathToImage, std::shared_ptr<LogicalDevice> pLogicalDevice, std::shared_ptr<PhysicalDevice> pPhysicalDevice, std::shared_ptr<CommandPool> pCommandPool)
    {
        this->pLogicalDevice = pLogicalDevice;
        this->pPhysicalDevice = pPhysicalDevice;
        this->pCommandPool = pCommandPool;
        auto properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(pathToImage.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        if (!pixels)
        {
            throw std::runtime_error("failed to load texture image!");
        }


        auto stagingBuffer = Buffer(imageSize,
                                           VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                           pLogicalDevice,
                                           pPhysicalDevice);

        void* data;
        stagingBuffer.MapMemory(imageSize, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(imageSize));
        stagingBuffer.UnmapMemory();

        stbi_image_free(pixels);

        imageInfo = ImageInfo(texWidth,
                              texHeight,
                              VK_FORMAT_R8G8B8A8_SRGB,
                              VK_IMAGE_TILING_OPTIMAL,
                              VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
        CreateImage(properties);

        transitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED,
                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        copyBufferToImage(stagingBuffer.buffer, image, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

        transitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                              VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    }

    ~Image()
    {
        vkDestroyImage(pLogicalDevice->device, image, nullptr);
        vkFreeMemory(pLogicalDevice->device, imageMemory, nullptr);
        pLogicalDevice.reset();
        pPhysicalDevice.reset();
        pCommandPool.reset();
    }

    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
    {
        auto commandBuffer = SingleTimeCommandBuffer(pCommandPool, pLogicalDevice);

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;

        region.imageOffset = {0, 0, 0};
        region.imageExtent = {
                width,
                height,
                1
        };

        vkCmdCopyBufferToImage(
                commandBuffer.commandBuffer,
                buffer,
                image,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1,
                &region
        );

        commandBuffer.EndAndSubmitBuffer();
    }

    void transitionImageLayout(VkFormat format, VkImageLayout oldLayout,
                                        VkImageLayout newLayout)
    {
        auto commandBuffer =SingleTimeCommandBuffer(pCommandPool, pLogicalDevice);

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.srcAccessMask = 0; // TODO
        barrier.dstAccessMask = 0; // TODO

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else
        {
            throw std::invalid_argument("unsupported layout transition!");
        }

        vkCmdPipelineBarrier(
                commandBuffer.commandBuffer,
                sourceStage, destinationStage,
                0,
                0, nullptr,
                0, nullptr,
                1, &barrier
        );

        commandBuffer.EndAndSubmitBuffer();
    }
};
