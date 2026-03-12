#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Termina/Core/Common.hpp>

namespace Termina {
    /// Manages the window and its properties.
    class Window
    {
    public:
        Window(int width, int height, const char* title);
        ~Window();

        bool IsOpen();
        void Update();

        /// Returns the window width in pixels.
        int32 GetWidth() const { return m_Width; }
        /// Returns the window height in pixels.
        int32 GetHeight() const { return m_Height; }

        /// Returns the framebuffer width in pixels.
        int32 GetPixelWidth() const { return m_PixelWidth; }
        /// Returns the framebuffer height in pixels.
        int32 GetPixelHeight() const { return m_PixelHeight; }

        GLFWwindow* GetHandle() const { return m_Window; }
    private:
        GLFWwindow* m_Window;

        int32 m_Width;
        int32 m_Height;
        int32 m_PixelWidth;
        int32 m_PixelHeight;
    };
}
