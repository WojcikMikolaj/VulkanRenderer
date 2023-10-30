//
// Created by Mikołaj Wójcik on 29.10.2023.
//

#include <iostream>
#include <GLFW/glfw3.h>
#include "Instance.h"

void Instance::checkExtensions(const char** extensionsToCheck, uint32_t extensionsToCheckCount)
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


std::vector<const char*> Instance::getRequiredExtensions(std::unique_ptr<ValidationLayers>& pValidationLayers)
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



