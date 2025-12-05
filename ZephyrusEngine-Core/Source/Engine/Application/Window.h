#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>
#include "Vector2D.h"
#include "Log.h"

/**
 * @brief Manages an SDL window and its dimensions for the editor.
 */
namespace Zephyrus::Application {
	class Window
	{
	public:
		Window(int pWidth = 800, int pHseight = 700, bool pIsResizable = false); // Constructs a window with the given width and height.
		Window(const Window&) = delete;
		void operator = (const Window&) = delete;

		Vector2D GetDimensions() const; // Returns the current window dimensions.
		void SetDimensions(const Vector2D& pDimensions);

		GLFWwindow* GetGlfwWindow() const; // Returns the underlying SDL_Window pointer.

		bool Open(const std::string& pTitle); // Opens the SDL window.
		void Close(); // Closes and destroys the SDL window.

	private:
		GLFWwindow* mGlfwWindow = nullptr;
		Vector2D mDimensions;
		bool mResizable = false;
	};
}