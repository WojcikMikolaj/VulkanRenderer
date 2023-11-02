//
// Created by "Mikołaj Wójcik" on 31.10.2023.
//
module;

#include <memory>
#include <stdexcept>
#include <vulkan/vulkan.h>

import Instance;
import ValidationLayers;

export module DebugMessenger;

export class DebugMessenger{
    VkDebugUtilsMessengerEXT debugMessenger;

    VkResult CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, VkDebugUtilsMessengerEXT* pDebugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
                pInstance->instance, "vkCreateDebugUtilsMessengerEXT");
        if (nullptr != func)
        {
            return func(pInstance->instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void DestroyDebugUtilsMessengerEXT()
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
                pInstance->instance, "vkDestroyDebugUtilsMessengerEXT");
        if (nullptr != func)
        {
            func(pInstance->instance, debugMessenger, pAllocator);
        }
    }


    std::shared_ptr<Instance> pInstance;
    const VkAllocationCallbacks* pAllocator;
public:
    DebugMessenger(std::shared_ptr<Instance> pInstance, std::unique_ptr<ValidationLayers>& pValidationLayers, const VkAllocationCallbacks* pAllocator)
    {
        this->pInstance = pInstance;
        this->pAllocator = pAllocator;

        if (!pValidationLayers->IsEnabled())
        {
            return;
        }

        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        ValidationLayers::populateDebugMessengerCreateInfo(createInfo);

        if (VK_SUCCESS != CreateDebugUtilsMessengerEXT(&createInfo, &debugMessenger))
        {
            throw std::runtime_error("Failed to set up debug messenger!");
        }
    }

    ~DebugMessenger()
    {
        DestroyDebugUtilsMessengerEXT();
    }
};
