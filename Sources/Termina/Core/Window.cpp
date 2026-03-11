#include "Window.hpp"
#include "Assert.hpp"

namespace Termina {
    Window::Window(int width, int height, const char* title)
    {
        TN_ASSERT(glfwInit(), "Failed to initialize GLFW");

        m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        TN_ASSERT(m_Window, "Failed to create GLFW window");
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    bool Window::IsOpen()
    {
        return !glfwWindowShouldClose(m_Window);
    }

    void Window::Update()
    {
        glfwPollEvents();
    }
}
