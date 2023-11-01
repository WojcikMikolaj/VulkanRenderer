//
// Created by "Mikołaj Wójcik" on 01.11.2023.
//

#ifndef VULKANRENDERER_SURFACE_H
#define VULKANRENDERER_SURFACE_H
#include "Instance.h"
#include <GLFW/glfw3.h>

class Surface
{
    std::shared_ptr<Instance> pInstance;
public:
    VkSurfaceKHR surface;
    Surface(std::shared_ptr<Instance> pInstance, std::unique_ptr<GLFWwindow*>& pWindow)
    {
        this->pInstance = pInstance;
        if (VK_SUCCESS != glfwCreateWindowSurface(this->pInstance->instance, *pWindow, nullptr, &surface))
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
#endif //VULKANRENDERER_SURFACE_H
