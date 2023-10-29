//
// Created by mikow on 29.10.2023.
//

#ifndef VULKANRENDERER_VALIDATIONLAYERS_H
#define VULKANRENDERER_VALIDATIONLAYERS_H

#include <vector>
#include <vulkan/vulkan.h>

class ValidationLayers
{
private:
    bool isEnabled;
    const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);
public:
    ValidationLayers(bool enableValidationLayers)
    {
        isEnabled = enableValidationLayers;
    }
    bool IsEnabled();
    bool CheckSupport();
    uint32_t LayersCount();
    const char* const* GetEnabledLayersNames();
    static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);


};
#endif //VULKANRENDERER_VALIDATIONLAYERS_H
