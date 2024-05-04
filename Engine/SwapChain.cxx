//
// Created by "Mikołaj Wójcik" on 01.11.2023.
//
module;

#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

import Window;
import Surface;
import PhysicalDevice;
import SwapChainSupportDetails;
import LogicalDevice;
import QueueFamilyIndices;
import ImageView;

export module SwapChain;

export class SwapChain
{
    std::shared_ptr<LogicalDevice> pLogicalDevice;

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
    {
        for (const auto& availableFormat : availableFormats)
        {
            if (VK_FORMAT_B8G8R8A8_SRGB == availableFormat.format
                && VK_COLOR_SPACE_SRGB_NONLINEAR_KHR == availableFormat.colorSpace)
            {
                return availableFormat;
            }
        }
        return availableFormats[0];
    }

    VkPresentModeKHR chooseSwapPresentMode(
            const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for (const auto& availablePresentMode : availablePresentModes)
        {
            if (VK_PRESENT_MODE_MAILBOX_KHR == availablePresentMode)
            {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D chooseSwapExtent(std::unique_ptr<Window>& pWindow, const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (std::numeric_limits<uint32_t>::max() != capabilities.currentExtent.width)
        {
            return capabilities.currentExtent;
        }
        else
        {
            int width, height;
            glfwGetFramebufferSize(pWindow->window, &width, &height);

            VkExtent2D actualExtent = {
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                                            capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                                             capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

public:
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    std::vector<std::unique_ptr<ImageView>> swapChainPImagesViews;
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

        if (indices.graphicsFamily.value() != indices.presentFamily.value())
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

        for (uint32_t i = 0; i < swapChainImages.size(); i++)
        {
            swapChainPImagesViews.push_back(std::move(std::make_unique<ImageView>(pLogicalDevice, swapChainImages[i], swapChainImageFormat)));
        }
    }

    ~SwapChain()
    {
        vkDestroySwapchainKHR(pLogicalDevice->device, swapChain, nullptr);
    }
};
