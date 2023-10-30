//
// Created by Mikołaj Wójcik on 29.10.2023.
//

#ifndef VULKANRENDERER_INSTANCE_H
#define VULKANRENDERER_INSTANCE_H

#include "AppInfo.h"
#include "Debug/ValidationLayers.h"
#include <memory>
#include <vulkan/vulkan.h>
#include <stdexcept>

class Instance
{
private:
    void checkExtensions(const char** extensionsToCheck, uint32_t extensionsToCheckCount);

    std::vector<const char*> getRequiredExtensions(std::unique_ptr<ValidationLayers>& pValidationLayers);

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
};

#endif //VULKANRENDERER_INSTANCE_H
