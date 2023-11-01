//
// Created by Mikołaj Wójcik on 31/10/2023.
//

#ifndef VULKANRENDERER_PHYSICALDEVICE_H
#define VULKANRENDERER_PHYSICALDEVICE_H
#include <vulkan/vulkan.h>
#include <memory>
#include "Instance.h"
#include "HelperStructs/QueueFamilyIndices.h"
#include "Surface.h"

const std::vector<const char*> deviceExtensions ={
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#if __APPLE__
        "VK_KHR_portability_subset",
#endif
};


class PhysicalDevice
{
    std::shared_ptr<Instance> pInstance;
    std::shared_ptr<Surface> pSurface;

    void PickPhysicalDevice();
    int RateDeviceSuitability(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    bool checkSwapChainAdequate(VkPhysicalDevice device);

public:
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    PhysicalDevice(std::shared_ptr<Instance> pInstance, std::shared_ptr<Surface> pSurface)
    {
        this->pInstance = pInstance;
        this->pSurface = pSurface;
        PickPhysicalDevice();
    }

    ~PhysicalDevice()
    {
        pSurface.reset();
        pInstance.reset();
    }

};
#endif //VULKANRENDERER_PHYSICALDEVICE_H
