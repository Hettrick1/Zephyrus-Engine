#include "Window.h"

Window::Window(int pWidth, int pHeight, bool pIsResizable) :
    mSdlWindow(nullptr), mDimensions({ (float)pWidth, (float)pHeight }), mResizable(pIsResizable)
{
}

Vector2D Window::GetDimensions() const
{
    return mDimensions;
}

void Window::SetDimensions(const Vector2D& pDimensions)
{
    mDimensions = pDimensions;
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
    if (mResizable)
    {
        mSdlWindow = SDL_CreateWindow(pTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, static_cast<int>(mDimensions.x), static_cast<int>(mDimensions.y), SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    }
    else
    {
        mSdlWindow = SDL_CreateWindow(pTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, static_cast<int>(mDimensions.x), static_cast<int>(mDimensions.y), SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    }
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
