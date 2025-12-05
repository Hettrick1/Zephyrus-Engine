#include "pch.h"
#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <dwmapi.h>
    #pragma comment(lib, "Dwmapi.lib")
    #define GLFW_EXPOSE_NATIVE_WIN32
    #include <GLFW/glfw3.h>
    #include <GLFW/glfw3native.h>
#endif
#include "Window.h"

namespace Zephyrus::Application {
    Window::Window(int pWidth, int pHeight, bool pIsResizable) :
        mGlfwWindow(nullptr), mDimensions({ (float)pWidth, (float)pHeight }), mResizable(pIsResizable)
    {
    }

    Vector2D Window::GetDimensions() const
    {
        return mDimensions;
    }

    void Window::SetDimensions(const Vector2D& pDimensions)
    {
        mDimensions = pDimensions;
        if (mGlfwWindow)
        {
            glfwSetWindowSize(mGlfwWindow, (int)pDimensions.x, (int)pDimensions.y);
        }
    }

    GLFWwindow* Window::GetGlfwWindow() const
    {
        return mGlfwWindow;
    }

    bool Window::Open(const std::string& pTitle)
    {
        if (!glfwInit())
        {
            ZP_CORE_ERROR("Failed to initialize GLFW");
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RED_BITS, 8);
        glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8);
        glfwWindowHint(GLFW_ALPHA_BITS, 8);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, mResizable ? GLFW_TRUE : GLFW_FALSE);

        mGlfwWindow = glfwCreateWindow(static_cast<int>(mDimensions.x), static_cast<int>(mDimensions.y), pTitle.c_str(), nullptr, nullptr);
        if (!mGlfwWindow)
        {
            ZP_CORE_ERROR("Failed to create GLFW window");
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(mGlfwWindow);
        
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            ZP_CORE_ERROR("Failed to initialize GLEW");
            return false;
        }
        glGetError();
        
        glfwSwapInterval(1);

#ifdef _WIN32
        HWND hwnd = glfwGetWin32Window(mGlfwWindow);
        BOOL dark = TRUE;
        DwmSetWindowAttribute(hwnd, 20, &dark, sizeof(dark));
#endif
        
        return true;

    }

    void Window::Close()
    {
        if (mGlfwWindow)
        {
            glfwDestroyWindow(mGlfwWindow);
            mGlfwWindow = nullptr;
        }
        glfwTerminate();
    }
}