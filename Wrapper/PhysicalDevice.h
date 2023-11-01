//
// Created by Mikołaj Wójcik on 31/10/2023.
//

#ifndef VULKANRENDERER_PHYSICALDEVICE_H
#define VULKANRENDERER_PHYSICALDEVICE_H
#include <vulkan/vulkan.h>
#include <memory>
#include "Instance.h"
#include "HelperStructs/QueueFamilyIndices.h"

class PhysicalDevice
{
    std::shared_ptr<Instance> pInstance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    void PickPhysicalDevice();
    int RateDeviceSuitability(VkPhysicalDevice device)
public:
    PhysicalDevice()
    {

    }
};
#endif //VULKANRENDERER_PHYSICALDEVICE_H
