//
// Created by "Mikołaj Wójcik" on 01.11.2023.
//

#ifndef VULKANRENDERER_WINDOW_H
#define VULKANRENDERER_WINDOW_H
#include <GLFW/glfw3.h>

class Window
{
public:
    GLFWwindow* window;
    Window(GLFWwindow* window)
    {
        this->window = window;
    }

    ~Window()
    {
        glfwDestroyWindow(window);
    }
};

#endif //VULKANRENDERER_WINDOW_H
