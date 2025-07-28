#include "Window.h"

Window::Window(int pWidth, int pHeight) : 
    mSdlWindow(nullptr), mDimensions({ (float)pWidth, (float)pHeight })
{
}

Vector2D Window::GetDimensions() const
{
    return mDimensions;
}

SDL_Window* Window::GetSdlWindow() const
{
    return mSdlWindow;
}

bool Window::Open(const std::string& pTitle)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        ZP_CORE_ERROR("Unable to initialize video");
        return false;
    }
    mSdlWindow = SDL_CreateWindow(pTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, static_cast<int>(mDimensions.x), static_cast<int>(mDimensions.y), SDL_WINDOW_OPENGL);
    if (!mSdlWindow)
    {
        ZP_CORE_ERROR("Failed to create window");
        return false;
    }
    return true;

}

void Window::Close()
{
    SDL_DestroyWindow(mSdlWindow);
}
