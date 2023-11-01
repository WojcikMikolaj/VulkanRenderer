//
// Created by "Mikołaj Wójcik" on 01.11.2023.
//

#ifndef VULKANRENDERER_LOGICALDEVICE_H
#define VULKANRENDERER_LOGICALDEVICE_H

#include <memory>
#include <vector>
#include <set>
#include <vulkan/vulkan.h>

#include "PhysicalDevice.h"
#include "Surface.h"

class LogicalDevice
{
public:
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    LogicalDevice(std::unique_ptr<PhysicalDevice>& pPhysicalDevice, std::shared_ptr<Surface>& pSurface, std::unique_ptr<ValidationLayers>& pValidationLayers)
    {
        QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(pPhysicalDevice->physicalDevice, pSurface->surface);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;
        //deviceFeatures.shaderFloat64 = VK_TRUE;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (pValidationLayers->IsEnabled())
        {
            createInfo.enabledLayerCount = pValidationLayers->LayersCount();
            createInfo.ppEnabledLayerNames = pValidationLayers->GetEnabledLayersNames();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        if (VK_SUCCESS != vkCreateDevice(pPhysicalDevice->physicalDevice, &createInfo, nullptr, &device))
        {
            throw std::runtime_error("Failed to create logical device!");
        }

        getDeviceQueues(pPhysicalDevice, pSurface);
    }

    ~LogicalDevice()
    {
        vkDestroyDevice(device, nullptr);
    }

    void getDeviceQueues(std::unique_ptr<PhysicalDevice>& pPhysicalDevice, std::shared_ptr<Surface>& pSurface)
    {
        QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(pPhysicalDevice->physicalDevice, pSurface->surface);
        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
    }

};
#endif //VULKANRENDERER_LOGICALDEVICE_H
