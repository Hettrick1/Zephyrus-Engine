#pragma once
#include <SDL.h>
#include "Vector2D.h"
#include "Log.h"

/**
 * @brief Manages an SDL window and its dimensions for the editor.
 */
class Window
{
public :
	Window(int pWidth = 800, int pHseight = 700); // Constructs a window with the given width and height.
	Window(const Window&) = delete;
	void operator = (const Window&) = delete;

	Vector2D GetDimensions() const; // Returns the current window dimensions.

	SDL_Window* GetSdlWindow() const; // Returns the underlying SDL_Window pointer.

	bool Open(const std::string& pTitle); // Opens the SDL window.
	void Close(); // Closes and destroys the SDL window.

private:
	SDL_Window* mSdlWindow = nullptr;
	Vector2D mDimensions;
};
