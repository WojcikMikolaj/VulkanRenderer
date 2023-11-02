//
// Created by Mikołaj Wójcik on 02.11.2023.
//
module;

#include <vulkan/vulkan.h>

export module ImageInfo;
export class ImageInfo
{
public:
    uint32_t width;
    uint32_t height;
    VkFormat format;
    VkImageTiling tiling;
    VkImageUsageFlags usageFlags;

    ImageInfo()
    {

    }

    ImageInfo(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
              VkImageUsageFlags usageFlags)
    {
        this->width = width;
        this->height = height;
        this->format = format;
        this->tiling = tiling;
        this->usageFlags = usageFlags;
    }

    ImageInfo(const ImageInfo& other): ImageInfo(width, height, format, tiling, usageFlags)
    {

    }

    ImageInfo(ImageInfo&& other): ImageInfo(width, height, format, tiling, usageFlags)
    {

    }

    VkImageCreateInfo GetCreateInfo()
    {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usageFlags;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        return imageInfo;
    }

    ImageInfo& operator=(const ImageInfo& other)
    {
        if(this != &other)
        {
            this->width = other.width;
            this->height = other.height;
            this->format = other.format;
            this->tiling = other.tiling;
            this->usageFlags = other.usageFlags;
            return *this;
        }
    }
};