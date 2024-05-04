//
// Created by Mikołaj Wójcik on 29.10.2023.
//
module;

#include <memory>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

import AppInfo;
import ValidationLayers;

export module Instance;

export class Instance
{
private:
    void checkExtensions(const char** extensionsToCheck, uint32_t extensionsToCheckCount)
    {
        uint32_t extensionCount = 0;

        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        std::cout << "available extensions:\n";

        for (const auto& extension : extensions)
        {
            std::cout << '\t' << extension.extensionName << '\n';
        }

        for (uint32_t i = 0; i < extensionsToCheckCount; i++)
        {
            std::cout << extensionsToCheck[i];
            bool found = false;
            for (const auto& extension : extensions)
            {
                if (std::string(extensionsToCheck[i]) == std::string(extension.extensionName))
                {
                    found = true;
                }
            }
            if (found)
            {
                std::cout << "\t found" << std::endl;
            }
            else
            {
                std::cout << "\t not found" << std::endl;
            }
        }
    }


    std::vector<const char*> getRequiredExtensions(std::unique_ptr<ValidationLayers>& pValidationLayers)
    {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (pValidationLayers->IsEnabled())
        {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

#if __APPLE__
        extensions.push_back("VK_KHR_get_physical_device_properties2");
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif

        return extensions;
    }



public:
    VkInstance instance;

    Instance(std::unique_ptr<AppInfo>& pAppInfo, std::unique_ptr<ValidationLayers>& pValidationLayers)
    {
        if (pValidationLayers->IsEnabled() && !pValidationLayers->CheckSupport())
        {
            throw std::runtime_error("Validation layers requested, but not available!");
        }

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = pAppInfo->GetAppName();
        appInfo.applicationVersion = VK_MAKE_VERSION(pAppInfo->GetAppMajorVersion(), pAppInfo->GetAppMinorVersion(), pAppInfo->GetAppPatchVersion());
        appInfo.pEngineName = pAppInfo->GetEngineName();
        appInfo.engineVersion = VK_MAKE_VERSION(pAppInfo->GetEngineMajorVersion(), pAppInfo->GetEngineMinorVersion(), pAppInfo->GetEnginePatchVersion());
        appInfo.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        auto extensions = getRequiredExtensions(pValidationLayers);

        checkExtensions(extensions.data(), static_cast<uint32_t>(extensions.size()));

        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (pValidationLayers->IsEnabled())
        {
            createInfo.enabledLayerCount = pValidationLayers->LayersCount();
            createInfo.ppEnabledLayerNames = pValidationLayers->GetEnabledLayersNames();

            ValidationLayers::populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
        }
        else
        {
            createInfo.enabledLayerCount = 0;

            createInfo.pNext = nullptr;
        }

#if __APPLE__
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

        if (VK_SUCCESS != vkCreateInstance(&createInfo, nullptr, &instance))
        {
            throw std::runtime_error("Failed to create instance!");
        }
    }

    ~Instance()
    {
        vkDestroyInstance(instance, nullptr);
    }
};
