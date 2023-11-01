//
// Created by "Mikołaj Wójcik" on 01.11.2023.
//
module;

#include <GLFW/glfw3.h>

export module Window;

export class Window
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
