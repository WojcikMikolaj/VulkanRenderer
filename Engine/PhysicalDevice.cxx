//
// Created by Mikołaj Wójcik on 31/10/2023.
//
module;

#include <memory>
#include <map>
#include <set>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <vulkan/vulkan.h>

import Instance;
import QueueFamilyIndices;
import Surface;
import SwapChainSupportDetails;

export module PhysicalDevice;

export const std::vector<const char*> deviceExtensions ={
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#if __APPLE__
        "VK_KHR_portability_subset",
#endif
};

export class PhysicalDevice
{
    std::shared_ptr<Instance> pInstance;
    std::shared_ptr<Surface> pSurface;

    void PickPhysicalDevice()
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

    int RateDeviceSuitability(VkPhysicalDevice device)
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

        QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(device, pSurface->surface);

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

    bool isDeviceSuitable(VkPhysicalDevice device)
    {
        return true;
    }

    bool checkDeviceExtensionSupport(VkPhysicalDevice device)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto& extension : availableExtensions)
        {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    bool checkSwapChainAdequate(VkPhysicalDevice device)
    {
        bool swapChainAdequate = false;
        SwapChainSupportDetails swapChainSupport = SwapChainSupportDetails::querySwapChainSupport(device, pSurface->surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        return swapChainAdequate;
    }

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

    VkPhysicalDeviceProperties GetProperties()
    {
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(physicalDevice, &properties);
        return properties;
    }
};
