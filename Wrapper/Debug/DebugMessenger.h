//
// Created by "Mikołaj Wójcik" on 31.10.2023.
//

#ifndef VULKANRENDERER_DEBUGMESSENGER_H
#define VULKANRENDERER_DEBUGMESSENGER_H

#include <memory>
#include <vulkan/vulkan.h>
#include "../Instance.h"

class DebugMessenger{
    VkDebugUtilsMessengerEXT debugMessenger;
public:
    DebugMessenger(std::unique_ptr<Instance>& pInstance, std::unique_ptr<ValidationLayers>& pValidationLayers)
    {
        if (!pValidationLayers->IsEnabled())
        {
            return;
        }

        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        ValidationLayers::populateDebugMessengerCreateInfo(createInfo);

        if (VK_SUCCESS != CreateDebugUtilsMessengerEXT(pInstance->instance, &createInfo, nullptr, &debugMessenger))
        {
            throw std::runtime_error("Failed to set up debug messenger!");
        }
    }

    static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                                 const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                 const VkAllocationCallbacks* pAllocator,
                                                 VkDebugUtilsMessengerEXT* pDebugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
                instance, "vkCreateDebugUtilsMessengerEXT");
        if (nullptr != func)
        {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    static void DestroyDebugUtilsMessengerEXT(std::unique_ptr<Instance>& pInstance, std::unique_ptr<DebugMessenger>& pDebugMessenger,
                                              const VkAllocationCallbacks* pAllocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
                pInstance->instance, "vkDestroyDebugUtilsMessengerEXT");
        if (nullptr != func)
        {
            func(pInstance->instance, pDebugMessenger->debugMessenger, pAllocator);
        }
    }
};

#endif //VULKANRENDERER_DEBUGMESSENGER_H
