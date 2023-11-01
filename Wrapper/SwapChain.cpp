//
// Created by Mikołaj Wójcik on 01.11.2023.
//
#include <algorithm>
#include "SwapChain.h"

VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
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

VkPresentModeKHR SwapChain::chooseSwapPresentMode(
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

VkExtent2D SwapChain::chooseSwapExtent(std::unique_ptr<Window>& pWindow, const VkSurfaceCapabilitiesKHR& capabilities)
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