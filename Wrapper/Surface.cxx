//
// Created by "Mikołaj Wójcik" on 01.11.2023.
//
module;

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
#include <stdexcept>
#include <vulkan/vulkan.h>

import Instance;
import Window;

export module Surface;

export class Surface
{
    std::shared_ptr<Instance> pInstance;
public:
    VkSurfaceKHR surface;
    Surface(std::shared_ptr<Instance> pInstance, std::unique_ptr<Window>& pWindow)
    {
        this->pInstance = pInstance;
        if (VK_SUCCESS != glfwCreateWindowSurface(this->pInstance->instance, pWindow->window, nullptr, &surface))
        {
            throw std::runtime_error("Failed to create window surface!");
        }
    }

    ~Surface()
    {
        vkDestroySurfaceKHR(pInstance->instance, surface, nullptr);
        pInstance.reset();
    }
};
