#include "Window.h"

Window::Window(int width, int height) : mSdlWindow(nullptr), mDimensions({ (float)width, (float)height })
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

bool Window::Open()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        Log::Error(LogType::Video, "Unable to initialize video");
        return false;
    }
    mSdlWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, static_cast<int>(mDimensions.x), static_cast<int>(mDimensions.y), SDL_WINDOW_OPENGL);
    if (!mSdlWindow)
    {
        Log::Error(LogType::System, "Failed to create window");
        return false;
    }
    return true;

}

void Window::Close()
{
    SDL_DestroyWindow(mSdlWindow);
}
