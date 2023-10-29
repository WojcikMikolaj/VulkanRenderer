//
// Created by Mikołaj Wójcik on 29.10.2023.
//

#ifndef VULKANRENDERER_SWAPCHAINSUPPORTDETAILS_H
#define VULKANRENDERER_SWAPCHAINSUPPORTDETAILS_H

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

#endif //VULKANRENDERER_SWAPCHAINSUPPORTDETAILS_H
