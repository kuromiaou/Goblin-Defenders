#pragma once

#include <GLFW/glfw3.h>

namespace Termina {
    class Window
    {
    public:
        Window(int width, int height, const char* title);
        ~Window();

        bool IsOpen();
        void Update();
    private:
        GLFWwindow* m_Window;
    };
}
