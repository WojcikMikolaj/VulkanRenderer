//
// Created by Mikołaj Wójcik on 31/10/2023.
//

#include "PhysicalDevice.h"
#include <vector>
#include <map>
#include <iostream>

void PhysicalDevice::PickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(pInstance->instance, &deviceCount, nullptr);

    if (0 == deviceCount)
    {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(pInstance->instance, &deviceCount, devices.data());

    std::multimap<int, VkPhysicalDevice> candidates;

    for (const auto& device : devices)
    {
        int score = RateDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }

    if (candidates.rbegin()->first > 0)
    {
        physicalDevice = candidates.rbegin()->second;
    }
    else
    {
        throw std::runtime_error("Failed to find suitable GPU!");
    }
}

int PhysicalDevice::RateDeviceSuitability(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    int score = 0;

    if (VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU == deviceProperties.deviceType)
    {
        score += 1000;
    }

    score += static_cast<int>(deviceProperties.limits.maxImageDimension2D);

    QueueFamilyIndices indices = findQueueFamilies(device);

    //kolejność powinna być: indices (chyba) -> deviceExtension (na pewno) -> swapChain
    if (!indices.isComplete()
        || !checkDeviceExtensionSupport(device)
        || !checkSwapChainAdequate(device)
        // || !deviceFeatures.geometryShader
        || !deviceFeatures.tessellationShader
        || !deviceFeatures.samplerAnisotropy
            )
    {
        std::cout << "Device: " << deviceProperties.deviceName << " Score: " << "0" << std::endl;
        return 0;
    }
    std::cout << "Device: " << deviceProperties.deviceName << " Score: " << score << std::endl;
    return score;
}

