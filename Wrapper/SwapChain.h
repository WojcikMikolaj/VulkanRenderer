//
// Created by "Mikołaj Wójcik" on 01.11.2023.
//

#ifndef VULKANRENDERER_SWAPCHAIN_H
#define VULKANRENDERER_SWAPCHAIN_H

#include <memory>
#include "PhysicalDevice.h"
#include "HelperStructs/SwapChainSupportDetails.h"
#include "LogicalDevice.h"

class SwapChain
{
    std::shared_ptr<LogicalDevice> pLogicalDevice;
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D chooseSwapExtent(std::unique_ptr<Window>& pWindow,const VkSurfaceCapabilitiesKHR &capabilities);

public:
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;

    SwapChain(std::shared_ptr<PhysicalDevice>& pPhysicalDevice, std::shared_ptr<Surface>& pSurface, std::shared_ptr<LogicalDevice> pLogicalDevice, std::unique_ptr<Window>& pWindow)
    {
        this->pLogicalDevice = pLogicalDevice;
        SwapChainSupportDetails swapChainSupport = SwapChainSupportDetails::querySwapChainSupport(pPhysicalDevice->physicalDevice, pSurface->surface);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(pWindow, swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0
            && imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = pSurface->surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(pPhysicalDevice->physicalDevice, pSurface->surface);
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        if (indices.graphicsFamily != indices.presentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; //optional
            createInfo.pQueueFamilyIndices = nullptr; //optional
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (VK_SUCCESS != vkCreateSwapchainKHR(this->pLogicalDevice->device, &createInfo, nullptr, &swapChain))
        {
            throw std::runtime_error("Failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(this->pLogicalDevice->device, swapChain, &imageCount, nullptr);
        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(this->pLogicalDevice->device, swapChain, &imageCount, swapChainImages.data());

        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent;
    }

    ~SwapChain()
    {
        vkDestroySwapchainKHR(pLogicalDevice->device, swapChain, nullptr);
    }
};
#endif //VULKANRENDERER_SWAPCHAIN_H
